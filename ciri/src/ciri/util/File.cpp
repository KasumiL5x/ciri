#include <ciri/util/File.hpp>
#include <sstream>
#include <ciri/ErrorCodes.hpp>

namespace ciri {
	File::File() {
	}

	File::File( const char* file ) {
		open(file);
	}

	File::~File() {
		close();
	}

	bool File::open( const char* file, bool append ) {
		_stream.open(file, std::fstream::in | std::fstream::out | (append ? (std::fstream::app) : std::fstream::trunc)); // try opening an existing file r/w
		if( !_stream.is_open() ) {
			// create and re-open the file otherwise
			_stream.open(file, std::fstream::in | std::fstream::out | std::fstream::trunc);
			_stream.open(file, std::fstream::in | std::fstream::out);
		}

		return _stream.is_open();
	}

	bool File::write( const std::string& msg ) {
		if( !isOpen() ) {
			return false;
		}
		_stream.write(msg.c_str(), msg.length());
		return true;
	}

	void File::close() {
		_stream.close();
	}

	std::string File::toString() {
		if( !_stream.is_open() ) {
			return "";
		}
		_stream.seekg(std::ios::beg); // go back to the beginning of the file
		std::stringstream ss;
		ss << _stream.rdbuf();
		return ss.str();
	}

	bool File::isOpen() const {
		return _stream.is_open();
	}
} // ciri