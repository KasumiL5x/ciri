#ifndef __ciri_gfx_itexture3d__
#define __ciri_gfx_itexture3d__

#include "TextureFlags.hpp"
#include "TextureFormat.hpp"
#include "../core/ErrorCodes.hpp"

namespace ciri {
	class ITexture3D {
	protected:
		ITexture3D( int flags ) {
		}

	public:
		virtual ~ITexture3D() {
		}

		/**
		 * Uninitializes the texture.
		 */
		virtual void destroy()=0;

		/**
		 * Sets or updates the texture data.
		 * @param width  Number of horizontal pixels.
		 * @param height Number of vertical pixels.
		 * @param depth  Number of stacked textures.
		 * @param data   Pointer to appropriately typed data forming width*height*depth pixels.
		 * @param format Format of the data to set; must match the format of the provided data pointer.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual ErrorCode setData( int width, int height, int depth, void* data, TextureFormat::Format format )=0;

		/**
		 * Gets the width of each texture
		 * @returns Width of each texture in pixels.
		 */
		virtual int getWidth() const=0;

		/**
		 * Gets the height of each texture.
		 * @returns Height of each texture in pixels.
		 */
		virtual int getHeight() const=0;

		/**
		 * Gets the number of stacked textures.
		 * @returns Number of stacked textures.
		 */
		virtual int getDepth() const=0;

		/**
		 * Gets the format of the textures.
		 * @return Format of the texture.
		 */
		virtual TextureFormat::Format getFormat() const=0;

		/**
		 * Writes each stacked texture to a separate TGA file.
		 * @param file Filename of the first layer.  Numbers will be appended to each following layer.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual ErrorCode writeToTGA( const char* file )=0;

		/**
		 * Writes each stacked texture to a separate DDS file.
		 * @param file Filename of the first layer.  Numbers will be appended to each following layer.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual ErrorCode writeToDDS( const char* file )=0;
	};
} // ciri

#endif /* __ciri_gfx_itexture3d__ */
