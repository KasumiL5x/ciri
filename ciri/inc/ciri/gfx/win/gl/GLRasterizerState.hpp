#ifndef __ciri_gfx_glrasterizerstate__
#define __ciri_gfx_glrasterizerstate__

#include <ciri/gfx/IRasterizerState.hpp>

namespace ciri {
	class GLRasterizerState : public IRasterizerState {
	public:
		GLRasterizerState();
		virtual ~GLRasterizerState();

		bool create( const RasterizerDesc& desc );
		virtual void destroy() override;

		const RasterizerDesc& getDesc() const;

	private:
		RasterizerDesc _desc;
	};
} // ciri

#endif /* __ciri_gfx_glrasterizerstate__ */
