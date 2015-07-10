#ifndef __ciri_file__
#define __ciri_file__

#include <fstream>
#include <string>

namespace ciri {
	class File {
	public:
		File();
		File( const char* file );
		~File();

		bool open( const char* file );
		void close();
		std::string toString() const;
		bool isOpen() const;

	private:
		std::ifstream _stream;
	};
} // ciri

#endif /* __ciri_file__ */