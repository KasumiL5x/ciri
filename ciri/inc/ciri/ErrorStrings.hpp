#ifndef __ciri_errorstrings__
#define __ciri_errorstrings__

namespace ciri {
	namespace err {
		const static char* SHADER_FILE_NOT_FOUND = "File not found: ";
		const static char* SHADER_PATH_NOT_FOUND = "Path not found: ";
		const static char* SHADER_COMPILE_FAILED = "Compile failed: ";
		const static char* SHADER_LINK_FAILED = "Failed to link: ";
	} // errstr
} // ciri

#endif /* __ciri_errorstrings__ */