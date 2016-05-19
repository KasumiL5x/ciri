#ifndef __ciri_graphics_GLDepthStencilState__
#define __ciri_graphics_GLDepthStencilState__

#include <ciri/graphics/IDepthStencilState.hpp>

namespace ciri { namespace graphics {

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

}}

#endif