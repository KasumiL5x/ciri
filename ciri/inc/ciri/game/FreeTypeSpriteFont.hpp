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
		virtual cc::Vec2i measureString( const std::string& str ) const override;
		virtual const std::unordered_map<char, SpriteFontGlyph> getLoadedCharacters() const override;
		virtual bool getGlyph( char character, SpriteFontGlyph& outGlyph ) override;

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