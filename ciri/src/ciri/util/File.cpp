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
	}

	bool File::open( const char* file ) {
		_stream.open(file, std::ifstream::in);
		return _stream.is_open();
	}

	void File::close() {
		_stream.close();
	}

	std::string File::toString() const {
		if( !_stream.is_open() ) {
			return "";
		}
		std::stringstream ss;
		ss << _stream.rdbuf();
		return ss.str();
	}

	bool File::isOpen() const {
		return _stream.is_open();
	}
} // ciri