#ifndef __ciri_gfx_glblendstate__
#define __ciri_gfx_glblendstate__

#include <ciri/gfx/IBlendState.hpp>

namespace ciri {
	class GLBlendState : public IBlendState {
	public:
		GLBlendState();
		virtual ~GLBlendState();

		bool create( const BlendDesc& desc );
		virtual void destroy() override;

		const BlendDesc& getDesc() const;

	private:
		BlendDesc _desc;
	};
} // ciri

#endif /* __ciri_gfx_glblendstate__ */