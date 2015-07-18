#ifndef __ciri_errorcodes__
#define __ciri_errorcodes__

#include <unordered_map>

namespace ciri {
	namespace err {
		enum ErrorCode {
			CIRI_OK = 0,
			CIRI_UNKNOWN_ERROR = 1001,
			CIRI_FILE_NOT_FOUND,
			CIRI_PATH_NOT_FOUND,
			CIRI_SHADER_COMPILE_FAILED,
			CIRI_SHADER_LINK_FAILED,
			CIRI_SHADER_INCOMPLETE
		};

		static std::unordered_map<ErrorCode, const char*> errorStrings = {
			{ CIRI_OK, "No error" },
			{ CIRI_UNKNOWN_ERROR, "Unknown error" },
			{ CIRI_FILE_NOT_FOUND, "File not found" },
			{ CIRI_PATH_NOT_FOUND, "Path not found" },
			{ CIRI_SHADER_COMPILE_FAILED, "Shader compile failed" },
			{ CIRI_SHADER_LINK_FAILED, "Shader link failed" },
			{ CIRI_SHADER_INCOMPLETE, "Shader data incomplete" }
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