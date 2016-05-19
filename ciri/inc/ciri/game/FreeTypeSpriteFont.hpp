#ifndef __ciri_game_FreeTypeSpriteFont__
#define __ciri_game_FreeTypeSpriteFont__

#include "ISpriteFont.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ciri {

	class IGraphicsDevice;

	class FreeTypeSpriteFont : public ISpriteFont {
	public:
		FreeTypeSpriteFont( const std::shared_ptr<IGraphicsDevice>& device );
		virtual ~FreeTypeSpriteFont();

		virtual ErrorCode loadFromFile( const char* file ) override;
		virtual ErrorCode preloadString( const std::string& str ) override;
		virtual void setSize( int size ) override;
		virtual int getSize() const override;
		virtual void setLineSpacing( int spacing ) override;
		virtual int getLineSpacing() const override;
		virtual void measureString( const std::string& str, int* outWidth, int* outHeight ) const override;
		virtual const std::unordered_map<char, SpriteFontGlyph> getLoadedCharacters() const override;
		virtual const SpriteFontGlyph& getGlyph( char character ) override;

	private:
		bool loadGlyph( char character );

	private:
		std::shared_ptr<IGraphicsDevice> _device;
		FT_Library _ftLibrary;
		FT_Face _ftFace;
		bool _fontLoaded;
		int _size;
		int _lineSpacing;
		std::unordered_map<char, SpriteFontGlyph> _glyphs;
	};

}

#endif