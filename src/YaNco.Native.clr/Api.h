#pragma once

#include <vector>
#include <vcclr.h>  
#include <msclr/marshal_cppstd.h>

#include "HandleWrappers.h"  
#include "NativeConverter.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace Dbosoft::YaNco;
using namespace msclr::interop;

namespace Dbosoft {
	namespace YaNco {
		namespace Native {

			RFC_RC SAP_API RFC_START_PROGRAM_Handler(RFC_CONNECTION_HANDLE rfcHandle, RFC_FUNCTION_HANDLE funcHandle, RFC_ERROR_INFO* errorInfoP);

			public ref class Api
			{
			public:
				static ConnectionHandle^ OpenConnection(IDictionary<String^, String^>^ connectionParams, [Out] RfcErrorInfo% errorInfo);
				static FunctionDescriptionHandle^ GetFunctionDescription(ConnectionHandle^ connectionHandle, String^ functionName, [Out] RfcErrorInfo% errorInfo);
				static FunctionDescriptionHandle^ GetFunctionDescription(FunctionHandle^ functionHandle, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetFunctionName(FunctionDescriptionHandle^ descriptionHandle, [Out] String^% functionName, [Out] RfcErrorInfo% errorInfo);

				static TypeDescriptionHandle^ GetTypeDescription(IDataContainerHandle^ dataContainer, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetTypeFieldCount(TypeDescriptionHandle^ descriptionHandle, [Out] int% count, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetTypeFieldDescription(TypeDescriptionHandle^ descriptionHandle, int index, [Out] RfcFieldInfo^% parameterInfo, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetTypeFieldDescription(TypeDescriptionHandle^ descriptionHandle, String^ name, [Out] RfcFieldInfo^% parameterInfo, [Out] RfcErrorInfo% errorInfo);

				static FunctionHandle^ CreateFunction(FunctionDescriptionHandle^ descriptionHandle, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetFunctionParameterCount(FunctionDescriptionHandle^ descriptionHandle, [Out] int% count, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetFunctionParameterDescription(FunctionDescriptionHandle^ descriptionHandle, int index, [Out] RfcParameterInfo^% parameterInfo, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetFunctionParameterDescription(FunctionDescriptionHandle^ descriptionHandle, String^ name, [Out] RfcParameterInfo^% parameterInfo, [Out] RfcErrorInfo% errorInfo);

				static RfcRc Invoke(ConnectionHandle^ connectionHandle, FunctionHandle^ functionHandle, [Out] RfcErrorInfo% errorInfo);

				static RfcRc GetStructure(IDataContainerHandle^ dataContainer, String^ name, [Out] StructureHandle^% structure, [Out] RfcErrorInfo% errorInfo);
				static RfcRc GetTable(IDataContainerHandle^ dataContainer, String^ name, [Out] TableHandle^% table, [Out] RfcErrorInfo% errorInfo);
				static TableHandle^ CloneTable(TableHandle^ tableHandle, [Out] RfcErrorInfo% errorInfo);

				static void AllowStartOfPrograms(ConnectionHandle^ connectionHandle, StartProgramDelegate^ callback, [Out] RfcErrorInfo% errorInfo);

				static RfcRc GetTableRowCount(TableHandle^ table, [Out] int% count, [Out] RfcErrorInfo% errorInfo);
				static StructureHandle^ GetCurrentTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo);
				static StructureHandle^ AppendTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo);
				static RfcRc MoveToNextTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo);
				static RfcRc MoveToFirstTableRow(TableHandle^ table, [Out] RfcErrorInfo% errorInfo);
					

				static RfcRc GetInt(IDataContainerHandle^ containerHandle, String^ name, [Out] int% value, [Out] RfcErrorInfo% errorInfo);
				static RfcRc SetInt(IDataContainerHandle^ containerHandle, String^ name, int value, [Out] RfcErrorInfo% errorInfo);

				static RfcRc GetLong(IDataContainerHandle^ containerHandle, String^ name, [Out] long long% value, [Out] RfcErrorInfo% errorInfo);
				static RfcRc SetLong(IDataContainerHandle^ containerHandle, String^ name, long long value, [Out] RfcErrorInfo% errorInfo);

				static RfcRc GetString(IDataContainerHandle^ containerHandle, String^ name, [Out] String^% value, [Out] RfcErrorInfo% errorInfo);
				static RfcRc SetString(IDataContainerHandle^ containerHandle, String^ name, String^ value, [Out] RfcErrorInfo% errorInfo);

				static RfcRc GetDateString(IDataContainerHandle^ containerHandle, String^ name, [Out] String^% value, [Out] RfcErrorInfo% errorInfo);
				static RfcRc SetDateString(IDataContainerHandle^ containerHandle, String^ name, String^ value, [Out] RfcErrorInfo% errorInfo);
					
				static RfcRc GetTimeString(IDataContainerHandle^ containerHandle, String^ name, [Out] String^% value, [Out] RfcErrorInfo% errorInfo);
				static RfcRc SetTimeString(IDataContainerHandle^ containerHandle, String^ name, String^ value, [Out] RfcErrorInfo% errorInfo);
					
				static RfcRc GetBytes(IDataContainerHandle^ containerHandle, String^ name, [Out] array<System::Byte>^% buffer, [Out] RfcErrorInfo% errorInfo);
				static RfcRc SetBytes(IDataContainerHandle^ containerHandle, String^ name, array<System::Byte>^ buffer, unsigned int bufferLength, [Out] RfcErrorInfo% errorInfo);

			internal:
				static RfcErrorInfo StartProgramHandler(String^ command);

			private:
				Api();
				static StartProgramDelegate^ StartProgramCallback;
			};

		}
	}
}
