#ifndef __ciri_log__
#define __ciri_log__

#include <string>
#include "File.hpp"

namespace ciri {
	class Log {
	public:
		Log();
		~Log();

		bool open( const char* file );
		bool print( const std::string& msg );
		bool printInfo( const std::string& msg );
		bool printWarning( const std::string& msg );
		bool printError( const std::string& msg );
		void close();

		void setAppendNewLine( bool val );
		void setPrefixTimestamp( bool val );

	private:
		std::string generateTimestampString() const;

	private:
		File _file;
		bool _appendNewLine;
		bool _prefixTimestamp;
	};
} // ciri

#endif /* __ciri_log__ */