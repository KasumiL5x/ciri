#ifndef __ciri_core_png__
#define __ciri_core_png__

namespace ciri {
	class PNG {
	public:
		PNG();
		~PNG();

		bool loadFromFile( const char* file );
		void destroy();

		unsigned char* getPixels() const;
		unsigned int getBytesPerPixel() const;
		unsigned int getBytesPerChannel() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;

	private:
		unsigned char* _pixels; // char* ?
		unsigned int _width;
		unsigned int _height;
		unsigned int _bitsPerChannel;
		unsigned int _bytesPerChannel;
		unsigned int _channelsPerPixel;
	};
} // ciri

#endif /* __ciri_core_png__ */