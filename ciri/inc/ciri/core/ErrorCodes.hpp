#ifndef __ciri_core_ErrorCodes__
#define __ciri_core_ErrorCodes__

#include <unordered_map>

namespace ciri {

enum class ErrorCode : int {
	CIRI_OK = 0,
	CIRI_UNKNOWN_ERROR						= -1,
	CIRI_FILE_NOT_FOUND						= -2,
	CIRI_PATH_NOT_FOUND						= -3,
	CIRI_SHADER_COMPILE_FAILED		=	-4,
	CIRI_SHADER_LINK_FAILED				=	-5,
	CIRI_SHADER_INCOMPLETE				=	-6,
	CIRI_SHADER_INVALID						=	-7,
	CIRI_INVALID_ARGUMENT					= -8,
	CIRI_STATIC_BUFFER_AS_DYNAMIC = -9,
	CIRI_BUFFER_MAP_FAILED				= -10,
	CIRI_BUFFER_CREATION_FAILED   = -11,
	CIRI_NOT_IMPLEMENTED					=	-12
};

static std::unordered_map<ErrorCode, const char*> errorStrings = {
	{ ErrorCode::CIRI_OK, "No error" },
	{ ErrorCode::CIRI_UNKNOWN_ERROR, "Unknown error" },
	{ ErrorCode::CIRI_FILE_NOT_FOUND, "File not found" },
	{ ErrorCode::CIRI_PATH_NOT_FOUND, "Path not found" },
	{ ErrorCode::CIRI_SHADER_COMPILE_FAILED, "Shader compile failed" },
	{ ErrorCode::CIRI_SHADER_LINK_FAILED, "Shader link failed" },
	{ ErrorCode::CIRI_SHADER_INCOMPLETE, "Shader data incomplete; shaders must have both a vertex and pixel shader" },
	{ ErrorCode::CIRI_SHADER_INVALID, "Shader is invalid" },
	{ ErrorCode::CIRI_INVALID_ARGUMENT, "Invalid argument" },
	{ ErrorCode::CIRI_STATIC_BUFFER_AS_DYNAMIC, "Attempted to treat a static buffer as a dynamic buffer" },
	{ ErrorCode::CIRI_BUFFER_MAP_FAILED, "Failed to map a buffer" },
	{ ErrorCode::CIRI_BUFFER_CREATION_FAILED, "Failed to create a buffer" },
	{ ErrorCode::CIRI_NOT_IMPLEMENTED, "Not yet implemented" }
};

static bool success( const ErrorCode& code ) {
	return ErrorCode::CIRI_OK == code;
}

static bool failed( const ErrorCode& code ) {
	return code != ErrorCode::CIRI_OK;
}

static const char* getErrorString( const ErrorCode& code ) {
	return errorStrings[code];
}

}

#endif