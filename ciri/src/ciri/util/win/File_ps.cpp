#include "File_ps.hpp"
#include <sstream>

namespace ciri {
	File_ps::File_ps() {
	}

	File_ps::~File_ps() {
	}

	bool File_ps::open( const char* file ) {
		_stream.open(file, std::ifstream::in);
		return _stream.is_open();
	}

	void File_ps::close() {
		_stream.close();
	}

	std::string File_ps::toString() const {
		if( !_stream.is_open() ) {
			return "";
		}
		std::stringstream ss;
		ss << _stream.rdbuf();
		return ss.str();
	}

	bool File_ps::isOpen() const {
		return _stream.is_open();
	}
} // ciri