#ifndef __ciri_glrasterizerstate__
#define __ciri_glrasterizerstate__

#include "../IRasterizerState.hpp"

namespace ciri {
	class GLRasterizerState : public IRasterizerState {
	public:
		GLRasterizerState();
		virtual ~GLRasterizerState();

		bool create( const RasterizerDesc& desc );
		virtual void destroy();

		const RasterizerDesc& getDesc() const;

	private:
		RasterizerDesc _desc;
	};
} // ciri

#endif /* __ciri_glrasterizerstate__ */