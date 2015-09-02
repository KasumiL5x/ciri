#ifndef __ciri_itexturecube__
#define __ciri_itexturecube__

#include "ITexture2D.hpp"
#include "../ErrorCodes.hpp"

namespace ciri {
	class ITextureCube {
	public:
		enum class Face {
			Right,  /**< Positive X. */
			Left,   /**< Negative X. */
			Top,    /**< Positive Y. */
			Bottom, /**< Negative Y. */
			Back,   /**< Positive Z. */
			Front   /**< Negative Z. */
		};

	public:
		ITextureCube() {
		}
		virtual ~ITextureCube() {
		}

		virtual err::ErrorCode load( ciri::ITexture2D* right, ciri::ITexture2D* left, ciri::ITexture2D* top, ciri::ITexture2D* bottom, ciri::ITexture2D* back, ciri::ITexture2D* front )=0;

		virtual ITexture2D* getTexture2D( Face face )=0;
	};
} // ciri

#endif/* __ciri_itexturecube__ */