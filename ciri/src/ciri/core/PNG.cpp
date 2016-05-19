#include <ciri/core/PNG.hpp>
#include <iostream>
#include <png.h>

using namespace ciri;

PNG::PNG()
	: _pixels(nullptr), _width(0), _height(0), _bitsPerChannel(0), _bytesPerChannel(0), _channelsPerPixel(0) {
}

PNG::~PNG() {
	destroy();
}

bool PNG::loadFromFile( const char* file, bool forceRGBA ) {
	if( nullptr == file ) {
		return false;
	}

	// load texture file into memory
	FILE* fp = nullptr;
	fp = std::fopen(file, "rb");
	if( nullptr == fp ) {
		return false;
	}

	// read a few bytes of the header to confirm this is a png.
	// the more bytes read, the more accurate the guess.
	const unsigned int HEADER_SIZE = 8;
	png_byte header[HEADER_SIZE];
	fread(header, 1, HEADER_SIZE, fp);
	if( png_sig_cmp(header, 0, HEADER_SIZE) != 0 ) {
		fclose(fp);
		return false;
	}

	// read png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if( nullptr == png_ptr ) {
		fclose(fp);
		return false;
	}

	// create info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if( nullptr == info_ptr ) {
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(fp);
		return false;
	}

	// set jmp
	if( setjmp(png_jmpbuf(png_ptr)) ) {
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(fp);
		return false;
	}

	// read png
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, HEADER_SIZE);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_SWAP_ENDIAN, nullptr);
		
	// get image data
	_width = png_get_image_width(png_ptr, info_ptr);
	_height = png_get_image_height(png_ptr, info_ptr);
	_bitsPerChannel = png_get_bit_depth(png_ptr, info_ptr);
	_bytesPerChannel = _bitsPerChannel / 8;
	_channelsPerPixel = png_get_channels(png_ptr, info_ptr);

	// store pixel data
	const unsigned int numBytes = _width * _height * _channelsPerPixel * _bytesPerChannel;
	_pixels = new unsigned char[numBytes];
	png_bytep pixelsPtr = (png_bytep)_pixels;
	png_bytepp rowPtrs = png_get_rows(png_ptr, info_ptr);
	for( unsigned int y = 0; y < _height; ++y ) {
		memcpy(pixelsPtr + y * png_get_rowbytes(png_ptr, info_ptr), rowPtrs[_height-y-1], png_get_rowbytes(png_ptr, info_ptr));
	}
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(fp);

	// if rgba was requested but the image is rgb, create a new buffer with an A channel and delete the old one.
	if( forceRGBA && _channelsPerPixel != 4 ) {
		// create new pixel array, copy old RGB over and append A
		unsigned char* newPixels = new unsigned char[_width * _height * 4 * _bytesPerChannel];
		int newPixelsOffset = 0;
		for( unsigned int i = 0; i < (_width * _height * _channelsPerPixel * _bytesPerChannel); i += (_channelsPerPixel*_bytesPerChannel) ) {
			memcpy(&newPixels[newPixelsOffset], &_pixels[i], _channelsPerPixel*_bytesPerChannel);
			newPixels[newPixelsOffset+_channelsPerPixel] = 255; // 100% alpha
			newPixelsOffset += 4;
		}
		// delete old pixels and assign new pixels
		delete[] _pixels;
		_pixels = newPixels;
		// update channel count to 4 (RGBA)
		_channelsPerPixel = 4;
	}

	return true;
}

void PNG::destroy() {
	if( _pixels != nullptr ) {
		delete[] _pixels;
		_pixels = nullptr;
	}
}

unsigned char* PNG::getPixels() const {
	return _pixels;
}

unsigned int PNG::getBytesPerPixel() const {
	return _channelsPerPixel;
}

unsigned int PNG::getBytesPerChannel() const {
	return _bytesPerChannel;
}

unsigned int PNG::getWidth() const {
	return _width;
}

unsigned int PNG::getHeight() const {
	return _height;
}

bool PNG::hasAlpha() const {
	return (4 == _channelsPerPixel);
}