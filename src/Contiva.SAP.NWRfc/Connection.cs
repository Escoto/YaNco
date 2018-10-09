﻿using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Contiva.Functional;
using LanguageExt;

namespace Contiva.SAP.NWRfc
{

    public class Connection : IDisposable
    {
        private readonly IAgent<AgentMessage, Either<RfcErrorInfo, object>> _stateAgent;
        private bool _disposed;

        public Connection(
            ConnectionHandle connectionHandle, 
            IRfcRuntime rfcRuntime)
        {

            _stateAgent = Agent.Start<ConnectionHandle, AgentMessage, Either<RfcErrorInfo, object>>(
                connectionHandle, (handle, msg) =>
                {
                    Console.WriteLine($"Agent: {System.Threading.Thread.CurrentThread.ManagedThreadId}");

                    if (handle == null)
                        return (null,
                            new RfcErrorInfo(RfcRc.RFC_INVALID_HANDLE, RfcErrorGroup.EXTERNAL_RUNTIME_FAILURE,
                                "Connection already destroyed", "", "", "", "", "", "", "", ""));

                    try
                    {
                        switch (msg)
                        {
                            case CreateFunctionMessage createFunctionMessage:
                            {
                                var result =
                                    from desc in rfcRuntime.GetFunctionDescription(handle,createFunctionMessage.FunctionName)
                                    from func in rfcRuntime.CreateFunction(desc)
                                    select new Function(func, rfcRuntime);

                                return (handle, result);

                            }

                            case InvokeFunctionMessage invokeFunctionMessage:
                            {
                                var result = rfcRuntime.Invoke(handle, invokeFunctionMessage.Function.Handle);
                                return (handle, result);

                            }

                            case AllowStartOfProgramsMessage allowStartOfProgramsMessage:
                            {
                                var result = rfcRuntime.AllowStartOfPrograms(handle,
                                    allowStartOfProgramsMessage.Callback);
                                return (handle, result);

                            }

                            case DisposeMessage _:
                            {
                                handle.Dispose();
                                return (null, new Either<RfcErrorInfo, object>());
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex);
                    }

                    throw new InvalidOperationException();
                });
        }

        public static Either<RfcErrorInfo,Connection> Create(IDictionary<string, string> connectionParams, IRfcRuntime runtime)
        {
            return runtime.OpenConnection(connectionParams).Map(handle => new Connection(handle, runtime));
        }

        public Task<Either<RfcErrorInfo, Unit>> CommitAndWait()
        {
            return CreateFunction("BAPI_TRANSACTION_COMMIT")
                .SetField("WAIT", "X")
                .MapAsync(f => f.Apply(InvokeFunction).Map(u => f))
                .HandleReturn()
                .MapAsync(f => Unit.Default);

        }

        public Task<Either<RfcErrorInfo, Unit>> Commit()
        {
            return CreateFunction("BAPI_TRANSACTION_COMMIT")
                .MapAsync(f => f.Apply(InvokeFunction).Map(u=>f))
                .HandleReturn()
                .MapAsync(f => Unit.Default);

        }

        public Task<Either<RfcErrorInfo, Unit>> Rollback()
        {
            return CreateFunction("BAPI_TRANSACTION_ROLLBACK")
                .BindAsync(InvokeFunction);

        }


        public Task<Either<RfcErrorInfo, Function>> CreateFunction(string name) =>
            _stateAgent.Tell(new CreateFunctionMessage(name)).BindAsync(r => (Either<RfcErrorInfo, Function>) r);

        public Task<Either<RfcErrorInfo, Unit>> InvokeFunction(Function function) =>
            _stateAgent.Tell(new InvokeFunctionMessage(function)).BindAsync(r => (Either<RfcErrorInfo, Unit>) r);

        public Task<Either<RfcErrorInfo, Unit>> AllowStartOfPrograms(StartProgramDelegate callback) =>
            _stateAgent.Tell(new AllowStartOfProgramsMessage(callback)).BindAsync(r => (Either<RfcErrorInfo, Unit>)r);


        private class AgentMessage
        {

        }

        private class CreateFunctionMessage : AgentMessage
        {
            public readonly string FunctionName;
            public CreateFunctionMessage(string functionName)
            {
                FunctionName = functionName;
            }

        }

        private class InvokeFunctionMessage : AgentMessage
        {
            public readonly Function Function;

            public InvokeFunctionMessage(Function function)
            {
                Function = function;
            }
        }

        private class AllowStartOfProgramsMessage : AgentMessage
        {
            public readonly StartProgramDelegate Callback;

            public AllowStartOfProgramsMessage(StartProgramDelegate callback)
            {
                Callback = callback;
            }
        }

        private class DisposeMessage : AgentMessage
        {
        }

        public void Dispose()
        {
            if (_disposed)
                return;

            _disposed = true;
            _stateAgent.Tell(new DisposeMessage());

        }
    }
}