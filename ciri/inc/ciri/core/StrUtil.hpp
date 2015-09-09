#ifndef __ciri_strutil__
#define __ciri_strutil__

#include <string>
#include <vector>
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

		static std::string wstr2str( const std::wstring& wstr ) {
			const int sLength = (int)wstr.length() + 1;
			const int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), sLength, 0, 0, 0, 0);
			char* buf = new char[len];
			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), sLength, buf, len, 0, 0);
			std::string str(buf);
			delete[] buf;
			return str;
		}

		static std::vector<std::string>* split( const char* str, char delim, std::vector<std::string>* outVec ) {
			do {
				const char* begin = str;
				while( *str != delim && *str ) {
					str++;
				}
				outVec->emplace_back(begin, str); // faster than push_back
			} while( 0 != *str++ );
			return outVec;
		}

		static int countCharactersInString( const char* str, char delim, int start ) {
			int count = 0;
			int index = start;
			while( true ) {
				const char c = str[index];
				if( '\0' == c  ) {
					break;
				}
				if( delim == c ) {
					count += 1;
				}
				index += 1;
			}
			return count;
		}
	} // strutil
} // ciri

#endif /* __ciri_strutil__ */