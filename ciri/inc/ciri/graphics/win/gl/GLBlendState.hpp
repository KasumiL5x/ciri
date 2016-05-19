#ifndef __ciri_graphics_GLBlendState__
#define __ciri_graphics_GLBlendState__

#include <ciri/graphics/IBlendState.hpp>

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

}

#endif