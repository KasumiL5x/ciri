#ifndef __ciri_textureformat__
#define __ciri_textureformat__
namespace ciri {
	class TextureFormat {
	public:
		enum Type {
			Color = 0,		// unsigned 32bit ARGB; 8 bits per channel
			RGB32_Float,
			RGBA32_Float
		};

		// MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetSize
		static int bytesPerPixel( Type type ) {
			switch( type ) {
				case Color: {
						return 4;
				}

				case RGB32_Float:
				case RGBA32_Float: {
						return 4;
				}

				default: {
					return -1;
				}
			}
		}
	};
} // ciri

#endif /* __ciri_textureformat__ */