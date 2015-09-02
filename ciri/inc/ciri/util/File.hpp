#ifndef __ciri_file__
#define __ciri_file__

#include <fstream>
#include <string>

namespace ciri {
	class File {
	public:
		struct Flags {
			enum FileFlags {
				None     = 0,
				ReadOnly = (1 << 0),
				Append   = (1 << 1)
			};
		};

	public:
		File();
		File( const char* file, int flags=Flags::None );
		~File();

		/**
		 * Opens or creates a file for writing.
		 * If an existing file is not present, but access is set to ReadOnly, a new file will not be created.  Otherwise, a new file will be created.
		 * 
		 * @param file  File to open or create.
		 * @param flags Flags for handling of the file.
		 * 
		 * @return True if the requested file was opened or created correctly.
		 */
		bool open( const char* file, int flags=Flags::None );

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
		int _flags;
	};
} // ciri

#endif /* __ciri_file__ */
