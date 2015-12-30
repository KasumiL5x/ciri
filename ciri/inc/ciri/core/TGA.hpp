#ifndef __ciri_core_tga__
#define __ciri_core_tga__

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

		/**
		 * Loads a TGA file into memory from file.
		 * @param file      TGA file to open.
		 * @param forceRGBA If true, and the source image does not contain alpha, the image will be remapped to RGBA.
		 * @returns Success of loading and parsing of the specified TGA file.
		 */
		bool loadFromFile( const char* file, bool forceRGBA );

		/**
		 * Cleans up allocated memory.
		 */
		void destroy();

		/**
		 * Gets the internal format of the image.
		 * @returns Internal Format of the image.
		 */
		Format getFormat() const;

		/**
		 * Gets the width of the image in pixels.
		 * @returns Width of image in pixels.
		 */
		int getWidth() const;

		/**
		 * Gets the height of the image in pixels.
		 * @returns Height of image in pixels.
		 */
		int getHeight() const;

		/**
		 * Gets a pointer to an array of the image's pixels.  If not loaded, this will be null.
		 * @returns Pointer to image pixels; nullptr upon error.
		 */
		unsigned char* getPixels() const;

		/**
		 * Gets if the pixels contain alpha.
		 * @returns True if the pixels contain alpha; false otherwise.
		 */
		bool hasAlpha() const;

		/**
		 * Writes pixels to a TGA file.
		 * @param[in] file The output image file.
		 * @param[in] width The width of the image in pixels.
		 * @param[in] height The height of the image in pixels.
		 * @param[in] pixels A pointer to an array of pixels.
		 * @param[in] format The format to write the TGA in.
		 * @param[in] swap Whether or not to swap the red and blue bits for each pixel (i.e. RGB->BGR or BGR->RGB).
		 * @remarks When writing data, the pixels pointer will be read width*height*bpp, where bpp is 3 for RGB formats or 4 for RGBA formats.
		 *          If swapping is requested, the provided pixels pointer is directly modified and a copy is not created.
		 */
		static bool writeToFile( const char* file, unsigned int width, unsigned int height, unsigned char* pixels, Format format, bool swap );

	private:
		Format _format;
		int _width;
		int _height;
		unsigned char* _pixels;
	};
} // ciri

#endif /* __ciri_core_tga__ */