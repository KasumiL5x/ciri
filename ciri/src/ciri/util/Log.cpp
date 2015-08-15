#include <ciri/util/Log.hpp>
#include <chrono>
#include <ctime>

namespace ciri {
	Log::Log()
		: _appendNewLine(true), _prefixTimestamp(true) {
	}

	Log::~Log() {
		close();
	}

	bool Log::open( const char* file ) {
		if( _file.isOpen() ) {
			return false;
		}

		return _file.open(file, false);
	}

	bool Log::print( const std::string& msg ) {
		if( !_file.isOpen() ) {
			return false;
		}

		std::string str =(_prefixTimestamp ? generateTimestampString() : "") +  msg + ((_appendNewLine) ? "\n" : "");
		return _file.write(str);
	}

	bool Log::printInfo( const std::string& msg ) {
		if( !_file.isOpen() ) {
			return false;
		}

		std::string str = (_prefixTimestamp ? generateTimestampString() : "") + "[INFO] " + msg + (_appendNewLine ? "\n" : "");
		return _file.write(str);
	}

	bool Log::printWarning( const std::string& msg ) {
		if( !_file.isOpen() ) {
			return false;
		}

		std::string str = (_prefixTimestamp ? generateTimestampString() : "") + "[WARNING] " + msg + (_appendNewLine ? "\n" : "");
		return _file.write(str);
	}

	bool Log::printError( const std::string& msg ) {
		if( !_file.isOpen() ) {
			return false;
		}

		std::string str = (_prefixTimestamp ? generateTimestampString() : "") + "[ERROR] " + msg + (_appendNewLine ? "\n" : "");
		return _file.write(str);
	}

	void Log::close() {
		_file.close();
	}

	void Log::setAppendNewLine( bool val ) {
		_appendNewLine = val;
	}

	void Log::setPrefixTimestamp( bool val ) {
		_prefixTimestamp = val;
	}

	std::string Log::generateTimestampString() const {
		const auto timeNow = std::chrono::system_clock::now();
		const std::time_t timeT = std::chrono::system_clock::to_time_t(timeNow);
		char buf[100];
		std::strftime(buf, sizeof(buf), "%c: ", std::localtime(&timeT));
		return buf;


		//return std::ctime(&timeT); // screw your ctime_s!  call the cops, idgaf!
	}

	std::unordered_map<Logs::Channel, Log> Logs::_logs;
	Log& Logs::get( Logs::Channel channel ) {
		return Logs::_logs[channel];
	}
} // ciri