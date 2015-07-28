#ifndef __ciri_tga__
#define __ciri_tga__

namespace ciri {
	class TGA {
	public:
		enum Format {
			RGB,
			RGBA
		};

	private:
#pragma pack(push, 1)
#pragma pack(1)
	struct Header {
		char idLength;
		char colorMapType;
		char imageType; // 2=uncompressed, 10=compressed, ignore the rest
		short firstEntryIndex;
		short colorMapLength;
		char colorMapEntrySize;
		short xOrigin;
		short yOrigin;
		short imageWidth;
		short imageHeight;
		char pixelDepth;
		char imageDesc;
	};
#pragma pack(pop)

	public:
		TGA();
		~TGA();

		bool loadFromFile( const char* file, bool forceRGBA );
		void destroy();

		Format getFormat() const;
		int getWidth() const;
		int getHeight() const;
		unsigned char* getPixels() const;

	private:
		Format _format;
		int _width;
		int _height;
		unsigned char* _pixels;
	};
} // ciri

#endif /* __ciri_tga__ */