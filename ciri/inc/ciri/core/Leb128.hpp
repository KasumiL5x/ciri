#ifndef __ciri_core_Leb128__
#define __ciri_core_Leb128__

#include <fstream>

namespace ciri { namespace leb128 {

/**
	* Decodes a signed integer encoded with LEB128-encoded from a stream.
	* Assumes the position in the stream is set and the data is valid.
	*
	* http://en.wikipedia.org/wiki/LEB128
	*/
	static int decodeStream( std::ifstream& data, int* outSize=nullptr ) {
		int result = 0;
		int shift = 0;
		int size = 0;
		while( true ) {
			// Read next byte.
			int b;
			data.read(reinterpret_cast<char*>(&b), sizeof(unsigned char));

			// Increment size.
			size += 1;

			// Or with low order 7 bits of the byte
			result |= (b & 0x7f) << shift;

			// If the high order of the bit is zero, we're done
			if( (b & 0x80) == 0 ) {
				break;
			}

			// Shift by 7 bytes
			shift += 7;
		}

		if( outSize ) {
			*outSize = size;
		}

		return result;
	}

}}

#endif