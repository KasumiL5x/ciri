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

		bool open( const char* file, bool append=true);
		bool write( const std::string& msg );
		void close();
		std::string toString();
		bool isOpen() const;

	private:
		std::fstream _stream;
	};
} // ciri

#endif /* __ciri_file__ */