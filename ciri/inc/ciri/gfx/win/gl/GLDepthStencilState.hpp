#ifndef __ciri_gfx_gldepthstencilstate__
#define __ciri_gfx_gldepthstencilstate__

#include <gl/glew.h>
#include <ciri/gfx/IDepthStencilState.hpp>

namespace ciri {
	class GLDepthStencilState : public IDepthStencilState {
	public:
		GLDepthStencilState();
		virtual ~GLDepthStencilState();

		bool create( const DepthStencilDesc& desc );
		virtual void destroy() override;

		const DepthStencilDesc& getDesc() const;

	private:
		DepthStencilDesc _desc;
	};
} // ciri

#endif /* __ciri_gfx_gldepthstencilstate__ */
