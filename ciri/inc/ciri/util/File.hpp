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

		/**
		 * Opens or creates a file for writing.
		 * 
		 * @param file   File to open or create.
		 * @param append If true, the file will be appended rather than replaced.
		 * 
		 * @return True if the requested file was opened or created correctly.
		 */
		bool open( const char* file, bool append=true);

		/**
		 * Writes a string to the file.
		 * 
		 * @param msg String to write.
		 * @return Success of writing.
		 */
		bool write( const std::string& msg );

		/**
		 * Closes the file handle.
		 */
		void close();

		/**
		 * Reads the entire file into a string.
		 * @return Entire file contents in a single string, or an empty string upon failure.
		 */
		std::string toString();

		/**
		 * Checks if the file stream is currently open for writing.
		 * @return True if open for writing.
		 */
		bool isOpen() const;

	private:
		std::fstream _stream;
	};
} // ciri

#endif /* __ciri_file__ */
