#ifndef __ciri_game_SpriteFontGlyph__
#define __ciri_game_SpriteFontGlyph__

#include <memory>

namespace ciri {

class ITexture2D;

class SpriteFontGlyph {
public:
	SpriteFontGlyph()
		: _width(0), _height(0), _bearingL(0), _bearingT(0), _advance(0), _texture(nullptr) {
	}
	SpriteFontGlyph( int w, int h, int bl, int bt, int adv, const std::shared_ptr<ITexture2D>& tex )
		: _width(w), _height(h), _bearingL(bl), _bearingT(bt), _advance(adv), _texture(tex) {
	}
	~SpriteFontGlyph() {
	}

	int width() const {
		return _width;
	}

	int height() const {
		return _height;
	}

	int bearingLeft() const {
		return _bearingL;
	}

	int bearingTop() const {
		return _bearingT;
	}

	int advance() const {
		return _advance;
	}

	std::shared_ptr<ITexture2D> texture() const {
		return _texture;
	}

private:
	int _width;  /**< Width in pixels. */
	int _height; /**< Height in pixels. */
	int _bearingL; /**< Offset from baseline to left side. */
	int _bearingT; /**< Offset from baseline to top side. */
	int _advance; /**< Offset to move to next glyph. */
	std::shared_ptr<ITexture2D> _texture; /**< Actual texture. */
};

}

#endif