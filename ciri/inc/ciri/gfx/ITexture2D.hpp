#ifndef __ciri_gfx_itexture2d__
#define __ciri_gfx_itexture2d__

#include "TextureFlags.hpp"
#include "TextureFormat.hpp"
#include "../core/ErrorCodes.hpp"

namespace ciri {
	class ITexture2D {
	protected:
		ITexture2D( int flags ) {
		}

	public:
		virtual ~ITexture2D() {
		}

		/**
		 * Uninitializes the texture.
		 */
		virtual void destroy()=0;

		/**
		 * Sets or updates the texture data.
		 * @param xOffset X offset of which to start setting data.
		 * @param yOffset Y offset of which to start setting data.
		 * @param width   Number of horizontal pixels from xOffset to set.
		 * @param height  Number of vertical pixels from yOffset to set.
		 * @param data    Pointer to appropriately typed data forming the region specified by the offsets and dimensions.
		 * @param format  Format of the data to set; must match the format of the provided data pointer.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual ErrorCode setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format )=0;

		/**
		 * Gets the width of the texture
		 * @return Width of the texture in pixels.
		 */
		virtual int getWidth() const=0;

		/**
		 * Gets the height of the texture.
		 * @return Height of the texture in pixels.
		 */
		virtual int getHeight() const=0;

		/**
		 * Writes the texture's contents to a TGA file.
		 * @param file TGA file to write to.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual ErrorCode writeToTGA( const char* file )=0;

		/**
		 * Writes the texture's contents to a DDS file.
		 * @param file DDS file to write to.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual ErrorCode writeToDDS( const char* file )=0;
	};
} // ciri

#endif /* __ciri_gfx_itexture2d__ */