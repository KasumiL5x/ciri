#ifndef __ciri_core_StrUtil__
#define __ciri_core_StrUtil__

#include <string>
#include <vector>
#include <codecvt>

namespace ciri { namespace core { namespace strutil {

	static std::wstring str2wstr_utf8( const std::string& str ) {
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return converterX.from_bytes(str);
	}

	static std::string wstr2str_utf8( const std::wstring& wstr ) {
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;
		return converterX.to_bytes(wstr);
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

}}}

#endif