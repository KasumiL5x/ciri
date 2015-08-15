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