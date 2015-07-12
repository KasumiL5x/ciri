#ifndef __ciri_strutil__
#define __ciri_strutil__

#include <string>
#include <Windows.h>

namespace ciri {
	namespace strutil {
		static std::wstring str2wstr( const std::string& str ) {
			// http://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
			const int sLength = (int)str.length() + 1;
			const int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), sLength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), sLength, buf, len);
			std::wstring wstr(buf);
			delete[] buf;
			return wstr;
		}
	} // strutil
} // ciri

#endif /* __ciri_strutil__ */