#include "File.hpp"

namespace ciri {
	File::File() {
	}

	File::File( const char* file ) {
		_platform.open(file);
	}

	File::~File() {
	}

	bool File::open( const char* file ) {
		return _platform.open(file);
	}

	void File::close() {
		_platform.close();
	}

	std::string File::toString() const {
		// warning: copy, copy
		return _platform.toString();
	}

	bool File::isOpen() const {
		return _platform.isOpen();
	}
} // ciri