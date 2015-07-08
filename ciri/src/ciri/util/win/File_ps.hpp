#ifndef __ciri_file_win__
#define __ciri_file_win__

#include <fstream>
#include <string>

namespace ciri {
	class File_ps {
	public:
		File_ps();
		~File_ps();

		bool open( const char* file );
		void close();
		std::string toString() const;

	private:
		std::ifstream _stream;
	};
} // ciri

#endif /* __ciri_file_win__ */