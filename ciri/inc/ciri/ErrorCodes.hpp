#ifndef __ciri_errorcodes__
#define __ciri_errorcodes__

#include <unordered_map>

namespace ciri {
	namespace err {
		enum ErrorCode : int {
			CIRI_OK = 0,
			CIRI_UNKNOWN_ERROR					= -1,
			CIRI_FILE_NOT_FOUND					= -2,
			CIRI_PATH_NOT_FOUND					= -3,
			CIRI_SHADER_COMPILE_FAILED	=	-4,
			CIRI_SHADER_LINK_FAILED			=	-5,
			CIRI_SHADER_INCOMPLETE			=	-6,
			CIRI_SHADER_INVALID					=	-7,
			CIRI_INVALID_ARGUMENT				= -8,
			CIRI_NOT_IMPLEMENTED				=	-9
		};

		static std::unordered_map<ErrorCode, const char*> errorStrings = {
			{ CIRI_OK, "No error" },
			{ CIRI_UNKNOWN_ERROR, "Unknown error" },
			{ CIRI_FILE_NOT_FOUND, "File not found" },
			{ CIRI_PATH_NOT_FOUND, "Path not found" },
			{ CIRI_SHADER_COMPILE_FAILED, "Shader compile failed" },
			{ CIRI_SHADER_LINK_FAILED, "Shader link failed" },
			{ CIRI_SHADER_INCOMPLETE, "Shader data incomplete" },
			{ CIRI_SHADER_INVALID, "Shader is invalid" },
			{ CIRI_INVALID_ARGUMENT, "Invalid argument" },
			{ CIRI_NOT_IMPLEMENTED, "Not yet implemented" }
		};

		static bool success( const ErrorCode& code ) {
			return CIRI_OK == code;
		}

		static bool failed( const ErrorCode& code ) {
			return code != CIRI_OK;
		}

		static const char* getString( const ErrorCode& code ) {
			return errorStrings[code];
		}
	}
} // ciri

#endif /* __ciri_errorcodes__ */