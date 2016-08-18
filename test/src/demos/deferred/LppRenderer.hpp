#ifndef __deferred_demo_LppRenderer__
#define __deferred_demo_LppRenderer__

#include <ciri/Graphics.hpp>

/* Light Prepass Deferred Renderer */
class LppRenderer {
public:
	LppRenderer();
	~LppRenderer();

	bool initialize( int width, int height );

private:
	bool createBuffers();
	
private:
	bool _initialized;
	int _width;
	int _height;

	std::shared_ptr<ciri::IRenderTarget2D> _positionTarget;
	std::shared_ptr<ciri::IRenderTarget2D> _normalTarget;
	std::shared_ptr<ciri::IRenderTarget2D> _depthTarget;
	std::shared_ptr<ciri::IRenderTarget2D> _lightTarget;
	std::shared_ptr<ciri::IRenderTarget2D> _sceneTarget;
	std::shared_ptr<ciri::IRenderTarget2D> _sceneDepthTarget;
	
};

#endif