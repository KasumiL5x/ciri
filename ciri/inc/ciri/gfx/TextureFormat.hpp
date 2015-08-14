#ifndef __ciri_textureformat__
#define __ciri_textureformat__

namespace ciri {
	struct TextureFormat {
		enum Format {
			Color = 0,		// unsigned 32bit ARGB; 8 bits per channel
			RGB32_Float,
			RGBA32_Float
		};

		// MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetSize
		static int bytesPerPixel( Format format ) {
			switch( format ) {
				case Color: {
						return 4;
				}

				case RGB32_Float: // 3???
				case RGBA32_Float: {
						return 4;
				}

				default: {
					return 0;
				}
			}
		}

		static bool hasAlpha( Format format ) {
			return (Color == format) ||
						 (RGBA32_Float == format);
		}
	};
} // ciri

#endif /* __ciri_textureformat__ */