#ifndef __ciri_file__
#define __ciri_file__

#include <ciri/Common.hpp>
#include CIRI_INCLUDE_PS(File_ps)
#include <string>

namespace ciri {
	class File {
	public:
		File();
		~File();

		bool open( const char* file );
		void close();
		std::string toString() const;

	private:
		File_ps _platform;
	};
} // ciri

#endif /* __ciri_file__ */