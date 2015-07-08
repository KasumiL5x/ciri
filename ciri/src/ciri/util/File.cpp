#include "File.hpp"

namespace ciri {
	File::File() {
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
} // ciri