#include <ciri/core/File.hpp>
#include <sstream>

using namespace ciri;

File::File()
	: _flags(Flags::None) {
}

File::File( const char* file, int flags ) {
	open(file, flags);
}

File::~File() {
	close();
}

bool File::open( const char* file, int flags ) {
	_flags = flags;

	unsigned int mode = 0;
	if( flags & Flags::ReadOnly ) {
		mode |= std::fstream::in;
	} else {
		mode |= (std::fstream::in | std::fstream::out);
		mode |= (flags & Flags::Append) ? std::fstream::app : std::fstream::trunc;
	}

	_stream.open(file, mode);
	if( !_stream.is_open() && !(flags & Flags::ReadOnly) ) {
		// create and re-open the file otherwise
		_stream.open(file, std::fstream::in | std::fstream::out | std::fstream::trunc);
		_stream.open(file, mode);
	}

	return _stream.is_open();
}

bool File::write( const std::string& msg ) {
	if( !isOpen() ) {
		return false;
	}

	if( _flags & Flags::ReadOnly ) {
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