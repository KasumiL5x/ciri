#include <ciri/game/FreeTypeSpriteFont.hpp>
#include <ciri/graphics/IGraphicsDevice.hpp>

using namespace ciri;

FreeTypeSpriteFont::FreeTypeSpriteFont( const std::shared_ptr<IGraphicsDevice>& device )
	: ISpriteFont(), _device(device), _fontLoaded(false), _size(14), _lineSpacing(14) {
}

FreeTypeSpriteFont::~FreeTypeSpriteFont() {
	if( _fontLoaded ) {
		FT_Done_Face(_ftFace);
		FT_Done_FreeType(_ftLibrary);
	}
}

ErrorCode FreeTypeSpriteFont::loadFromFile( const char* file ) {
	if( _fontLoaded ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: already initialized font
		// todo: possibly replace loaded font?
	}

	// initialize freetype library
	if( FT_Init_FreeType(&_ftLibrary) != FT_Err_Ok ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: failed to load font library
	}

	if( FT_New_Face(_ftLibrary, file, 0, &_ftFace) != FT_Err_Ok ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: failed to load font file
	}

	// todo: some default size setting somewhere
	FT_Set_Pixel_Sizes(_ftFace, 0, _size);

	_fontLoaded = true;

	return ErrorCode::CIRI_OK;
}

ErrorCode FreeTypeSpriteFont::preloadString( const std::string& str ) {
	if( !_fontLoaded ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: not loaded
	}

	for( auto c : str ) {
		if( !loadGlyph(c) ) {
			return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: failed to load character
		}
	}

	return ErrorCode::CIRI_OK;
}

void FreeTypeSpriteFont::setSize( int size ) {
	if( !_fontLoaded ) {
		return;
	}

	_size = size;
	FT_Set_Pixel_Sizes(_ftFace, 0, size);
}

int FreeTypeSpriteFont::getSize() const {
	return _size;
}

void FreeTypeSpriteFont::setLineSpacing( int spacing ) {
	_lineSpacing = spacing;
}

int FreeTypeSpriteFont::getLineSpacing() const {
	return _lineSpacing;
}

cc::Vec2i FreeTypeSpriteFont::measureString( const std::string& str ) const {
	if( str.empty() ) {
		return cc::Vec2i::zero();
	}

	cc::Vec2i size;

	int thisLineHeight = getLineSpacing();
	for( const auto& c : str ) {
		if( '\r' == c ) {
			continue;
		}

		if( '\n' == c ) {
			size.y += thisLineHeight; // add previous line height
			thisLineHeight = getLineSpacing(); // reset line height
			continue;
		}

		const auto glyph = _glyphs.find(c);
		if( _glyphs.end() == glyph ) {
			continue;
		}

		size.x += (glyph->second.advance() >> 6);
		thisLineHeight = (glyph->second.height() > thisLineHeight) ? glyph->second.height() : thisLineHeight;
	}
	size.y += thisLineHeight; // append remaining line height

	//bool lineFinished = false;
	//int lineSpacing = getLineSpacing();
	//for( const auto& c : str ) {
	//	// ignore carriage return
	//	if( '\r' == c ) {
	//		continue;
	//	}

	//	// handle newline
	//	if( '\n' == c ) {
	//		size.y += getLineSpacing();
	//	}

	//	// find glyph; ignore non-existent ones (not loading them, hence not using getGlyph)
	//	const auto existing = _glyphs.find(c);
	//	if( _glyphs.end() == existing ) {
	//		continue;
	//	}

	//	// append line's height based on glyph
	//	lineSpacing = existing->second.height() > lineSpacing ? existing->second.height() : lineSpacing;

	//	size.x += (existing->second.advance() >> 6);
	//}
	return size;
}

const std::unordered_map<char, SpriteFontGlyph> FreeTypeSpriteFont::getLoadedCharacters() const {
	return _glyphs;
}

bool FreeTypeSpriteFont::getGlyph( char character, SpriteFontGlyph& outGlyph ) {
	const auto existing = _glyphs.find(character);
	if( _glyphs.end() == existing ) {
		if( !loadGlyph(character) ) {
			return false;
		}
	}
	outGlyph = _glyphs[character];
	return true;
}

bool FreeTypeSpriteFont::loadGlyph( char character ) {
	if( FT_Load_Char(_ftFace, character, FT_LOAD_RENDER) != FT_Err_Ok ) {
		return false;
	}

	const int width = _ftFace->glyph->bitmap.width;
	const int height = _ftFace->glyph->bitmap.rows;
	const int bearingL = _ftFace->glyph->bitmap_left;
	const int bearingT = _ftFace->glyph->bitmap_top;
	const int advance = _ftFace->glyph->advance.x;
	float* floatBuffer = new float[width*height*4];
	for( int y = 0; y < height; ++y ) {
		for( int x = 0; x < width; ++x ) {
			const int glyphPixel = (height-1-y) * width + x;
			const float val = static_cast<float>(_ftFace->glyph->bitmap.buffer[glyphPixel]) / 255.0f;

			const int floatPixel = (y * width * 4) + x * 4;
			floatBuffer[floatPixel+0] = val;
			floatBuffer[floatPixel+1] = val;
			floatBuffer[floatPixel+2] = val;
			floatBuffer[floatPixel+3] = val;
		}
	}
	const auto texture = _device->createTexture2D(width, height, ciri::TextureFormat::RGBA32_Float, 0, floatBuffer);//_ftFace->glyph->bitmap.buffer);
	delete[] floatBuffer;
	floatBuffer = nullptr;
	SpriteFontGlyph glyph(width, height, bearingL, bearingT, advance, texture);
	_glyphs.insert(std::make_pair(character, glyph));
	return true;
}