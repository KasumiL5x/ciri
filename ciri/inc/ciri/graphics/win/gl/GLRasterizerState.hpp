#ifndef __ciri_graphics_GLRasterizerState__
#define __ciri_graphics_GLRasterizerState__

#include <ciri/graphics/IRasterizerState.hpp>

namespace ciri { namespace graphics {

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

}}

#endif