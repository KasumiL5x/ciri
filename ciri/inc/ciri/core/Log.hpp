#ifndef __ciri_core_log__
#define __ciri_core_log__

#include <string>
#include <unordered_map>
#include "File.hpp"

namespace ciri {
	class Log {
	public:
		Log();
		~Log();


		/**
		 * Writes a message to the log.
		 * 
		 * @param msg Message to write.
		 */
		void print( const std::string& msg );

		/**
		 * Writes a message to the log, prefixed with [INFO]. 
		 * 
		 * @param msg Message to write.
		 */
		void printInfo( const std::string& msg );

		/**
		 * Writes a message to the log, prefixed with [WARNING].
		 * 
		 * @param msg Message to write.
		 */
		void printWarning( const std::string& msg );

		/**
		 * Writes a message to the log, prefixed with [ERROR].
		 * 
		 * @param msg Mesage to write.
		 */
		void printError( const std::string& msg );

		/**
		 * Sets the output file of the log.
		 * If the file already exists, it will be replaced; otherwise, it will be created.
		 * 
		 * @param file File to use for log output.
		 * @return Success of opening the specified file for writing.
		 */
		bool setFile( const char* file );

		/**
		 * Closes the file handle.
		 */
		void closeFile();

		/**
		 * Sets whether to append a new line a the end of messages.
		 * 
		 * @param val If true, a new line '\n' will be appended to all messages.
		 */
		void setAppendNewLine( bool val );

		/**
		 * Sets whether to prefix all messages with a timestamp.
		 * 
		 * @param val If true, a timestamp will be prefixed to all messages.
		 */
		void setPrefixTimestamp( bool val );

		/**
		 * Sets whether to additionally log to standard output.
		 * 
		 * @param val If true, output will be sent to standard output in addition to the file.
		 */
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

#endif /* __ciri_core_log__ */