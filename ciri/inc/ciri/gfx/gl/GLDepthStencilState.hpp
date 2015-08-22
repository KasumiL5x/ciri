#ifndef __ciri_gldepthstencilstate__
#define __ciri_gldepthstencilstate__

#include <gl/glew.h>
#include "../IDepthStencilState.hpp"

namespace ciri {
	class GLDepthStencilState : public IDepthStencilState {
	public:
		GLDepthStencilState();
		virtual ~GLDepthStencilState();

		bool create( const DepthStencilDesc& desc );
		virtual void destroy();

		const DepthStencilDesc& getDesc() const;

		GLenum ciriToGlOp( const StencilOperation::Operation op ) const;

	private:
		DepthStencilDesc _desc;
	};
}

#endif /* __ciri_gldepthstencilstate__ */