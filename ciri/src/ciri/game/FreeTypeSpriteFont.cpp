#include <ciri/game/FreeTypeSpriteFont.hpp>
#include <ciri/graphics/IGraphicsDevice.hpp>

using namespace ciri;

FreeTypeSpriteFont::FreeTypeSpriteFont( const std::shared_ptr<IGraphicsDevice>& device )
	: ISpriteFont(), _device(device), _fontLoaded(false), _size(14) {
}

FreeTypeSpriteFont::~FreeTypeSpriteFont() {
	FT_Done_Face(_ftFace);
	FT_Done_FreeType(_ftLibrary);
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
	throw;
}

int FreeTypeSpriteFont::getLineSpacing() const {
	return _lineSpacing;
}

void FreeTypeSpriteFont::measureString( const std::string& str, int* outWidth, int* outHeight ) const {
	throw;
}

const std::unordered_map<char, SpriteFontGlyph> FreeTypeSpriteFont::getLoadedCharacters() const {
	return _glyphs;
}

const SpriteFontGlyph& FreeTypeSpriteFont::getGlyph( char character ) {
	const auto existing = _glyphs.find(character);
	if( _glyphs.end() == existing ) {
		if( !loadGlyph(character) ) {
			throw; // todo: don't do it this way
		}
	}
	return _glyphs[character];
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
	const auto texture = _device->createTexture2D(width, height, ciri::TextureFormat::R32_UINT, 0, _ftFace->glyph->bitmap.buffer);
	SpriteFontGlyph glyph(width, height, bearingL, bearingT, advance, texture);
	_glyphs.insert(std::make_pair(character, glyph));
	return true;
}