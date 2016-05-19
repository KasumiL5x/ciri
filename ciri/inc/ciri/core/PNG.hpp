#ifndef __ciri_core_PNG__
#define __ciri_core_PNG__

namespace ciri { namespace core {

class PNG {
public:
	PNG();
	~PNG();

	/**
		* Loads a PNG file into memory from file.
		* @param file      PNG file to open.
		* @param forceRGBA If true, and the source image does not contain alpha, the image will be remapped to RGBA.
		* @returns Success of loading and parsing of the specified PNG file.
		*/
	bool loadFromFile( const char* file, bool forceRGBA );

	/**
		* Cleans up allocated memory.
		*/
	void destroy();

	/**
		* Gets a pointer to an array of the image's pixels.  If not loaded, this will be null.
		* @returns Pointer to image pixels; nullptr upon error.
		*/
	unsigned char* getPixels() const;

	/**
		* Gets the number of bytes per pixel.  For RGB images, this will be 3; for RGBA, 4.
		* @returns Number of bytes per pixel.
		*/
	unsigned int getBytesPerPixel() const;

	/**
		* Gets the number of bytes per channel.  This is usually 1 byte.
		* @returns Number of bytes per channel.
		*/
	unsigned int getBytesPerChannel() const;

	/**
		* Gets the width of the image in pixels.
		* @returns Width of image in pixels.
		*/
	unsigned int getWidth() const;

	/**
		* Gets the height of the image in pixels.
		* @returns Height of image in pixels.
		*/
	unsigned int getHeight() const;

	/**
		* Gets if the pixels contain alpha.
		* @returns True if pixels contain alpha; false otherwise.
		*/
	bool hasAlpha() const;

private:
	unsigned char* _pixels;
	unsigned int _width;
	unsigned int _height;
	unsigned int _bitsPerChannel;
	unsigned int _bytesPerChannel;
	unsigned int _channelsPerPixel;
};

}}

#endif