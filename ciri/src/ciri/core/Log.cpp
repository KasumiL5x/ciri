#include <ciri/core/Log.hpp>
#include <chrono>
#include <ctime>

using namespace ciri::core;

Log::Log()
	: _appendNewLine(true), _prefixTimestamp(true), _logToStd(true) {
}

Log::~Log() {
	closeFile();
}

void Log::print( const std::string& msg ) {
	std::string str =(_prefixTimestamp ? generateTimestampString() : "") +  msg + ((_appendNewLine) ? "\n" : "");
	if( _logToStd ) {
		printf("%s", str.c_str());
	}
	if( _file.isOpen() ) {
		_file.write(str);
	}
}

void Log::printInfo( const std::string& msg ) {
	std::string str = (_prefixTimestamp ? generateTimestampString() : "") + "[INFO] " + msg + (_appendNewLine ? "\n" : "");
	if( _logToStd ) {
		printf("%s", str.c_str());
	}
	if( _file.isOpen() ) {
		_file.write(str);
	}
}

void Log::printWarning( const std::string& msg ) {
	std::string str = (_prefixTimestamp ? generateTimestampString() : "") + "[WARNING] " + msg + (_appendNewLine ? "\n" : "");
	if( _logToStd ) {
		printf("%s", str.c_str());
	}
	if( _file.isOpen() ) {
		_file.write(str);
	}
}

void Log::printError( const std::string& msg ) {
	std::string str = (_prefixTimestamp ? generateTimestampString() : "") + "[ERROR] " + msg + (_appendNewLine ? "\n" : "");
	if( _logToStd ) {
		printf("%s", str.c_str());
	}
	if( _file.isOpen() ) {
		_file.write(str);
	}
}

bool Log::setFile( const char* file ) {
	if( _file.isOpen() ) {
		return false;
	}

	return _file.open(file, false);
}

void Log::closeFile() {
	_file.close();
}

void Log::setAppendNewLine( bool val ) {
	_appendNewLine = val;
}

void Log::setPrefixTimestamp( bool val ) {
	_prefixTimestamp = val;
}

void Log::setLogToStd( bool val ) {
	_logToStd = val;
}

std::string Log::generateTimestampString() const {
	const auto timeNow = std::chrono::system_clock::now();
	const std::time_t timeT = std::chrono::system_clock::to_time_t(timeNow);
	char buf[100];
	std::strftime(buf, sizeof(buf), "%c: ", std::localtime(&timeT));
	return buf;
}

std::unordered_map<Logs::Channel, Log> Logs::_logs;
Log& Logs::get( Logs::Channel channel ) {
	return Logs::_logs[channel];
}