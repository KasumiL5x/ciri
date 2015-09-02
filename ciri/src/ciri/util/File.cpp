#include <ciri/util/File.hpp>
#include <sstream>
#include <ciri/ErrorCodes.hpp>

namespace ciri {
	File::File()
		: _access(ReadWrite) {
	}

	File::File( const char* file, Access access ) {
		open(file, false, access); // todo: just use bit flags for access instead...
	}

	File::~File() {
		close();
	}

	bool File::open( const char* file, bool append, Access access ) {
		_access = access;

		unsigned int mode = 0;
		if( ReadOnly == access ) {
			mode |= std::fstream::in;
		} else if( ReadWrite == access ) {
			mode |= std::fstream::in | std::fstream::out;
		}
		if( access != ReadOnly ) {
			if( append ) {
				mode |= std::fstream::app;
			} else {
				mode |= std::fstream::trunc;
			}
		}

		_stream.open(file, mode);
		if( !_stream.is_open() && (_access != ReadOnly) ) {
			// create and re-open the file otherwise
			_stream.open(file, std::fstream::in | std::fstream::out | std::fstream::trunc);
			_stream.open(file, std::fstream::in | std::fstream::out);
		}

		return _stream.is_open();
	}

	bool File::write( const std::string& msg ) {
		if( !isOpen() || (_access != ReadWrite) ) {
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