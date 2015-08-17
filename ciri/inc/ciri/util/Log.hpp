#ifndef __ciri_log__
#define __ciri_log__

#include <string>
#include <unordered_map>
#include "File.hpp"

namespace ciri {
	class Log {
	public:
		Log();
		~Log();

		void print( const std::string& msg );
		void printInfo( const std::string& msg );
		void printWarning( const std::string& msg );
		void printError( const std::string& msg );

		bool setFile( const char* file );
		void closeFile();
		void setAppendNewLine( bool val );
		void setPrefixTimestamp( bool val );
		void setLogToStd( bool val );

	private:
		std::string generateTimestampString() const;

	private:
		File _file;
		bool _appendNewLine;
		bool _prefixTimestamp;
		bool _logToStd;
	};

	class Logs {
	public:
		enum Channel {
			Default,
			Debug,
			Custom
		};

		static Log& get( Channel channel );

	private:
		static std::unordered_map<Channel, Log> _logs;
	};
} // ciri

#endif /* __ciri_log__ */