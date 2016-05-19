#include "Gridlr.hpp"
#include <cctype>
#include <array>
#include <cc/Random.hpp>

//
// random note about spritefonts!  the idea is something like the following:
//
// creation:
//	std::shared_ptr<ciri::ISpriteFont> spriteFont = device->createSpriteFont("Helvetica.ttf", any, other, default, params);
//	(or spriteFont->loadFromFile("..."); spriteFont->setSize(...); and so on).
// 
// drawing:
//	device->drawString(spriteFont, "The String To Draw", color, position, rotation, pivot, states, custom shader, whatever...);
//
// can measure strings in pixels like this:
//	string score = "Player Score:";
//	vec2 size = spriteFont.measureString(score);
//	device->drawString(spriteFont, player_score_str, score_pos + vec2(size.x + spacing, 0), ...);
//

Gridlr::Gridlr()
	: App(), _grid(nullptr), _blendState(nullptr), _samplerState(nullptr), _depthStencilState(nullptr), _rasterizerState(nullptr), _cellTexture(nullptr),
		_isDragging(false) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Gridlr";
}

Gridlr::~Gridlr() {
}

void Gridlr::onInitialize() {
	App::onInitialize();

	// cell off grid to start
	_lastMouseCell.x = -1;
	_lastMouseCell.y = -1;

	// make new empty grid
	_grid = std::make_unique<gridlr::BlockGrid>();
	_grid->create(8, 8);

	// enable chains
	_grid->enableChain(gridlr::BlockState::One, *_grid->get(0, 0), *_grid->get(4, 4));
	_grid->enableChain(gridlr::BlockState::Two, *_grid->get(4, 6), *_grid->get(6, 2));
	_grid->enableChain(gridlr::BlockState::Three, *_grid->get(1, 4), *_grid->get(5, 6));

	// debug print grid
	_grid->print();

	// print driver info
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// create spritebatch
	printf(_spriteBatch.create(graphicsDevice()) ? "Initialized SpriteBatch.\n" : "Failed to initialize SpriteBatch.\n");

	// create states
	_blendState = graphicsDevice()->getDefaultBlendAlpha();
	_samplerState = graphicsDevice()->createSamplerState(ciri::SamplerDesc());
	_depthStencilState = graphicsDevice()->getDefaultDepthStencilNone();
	_rasterizerState = graphicsDevice()->getDefaultRasterCounterClockwise();
}

void Gridlr::onLoadContent() {
	App::onLoadContent();

	// load grid texture
	ciri::PNG cellPng;
	if( cellPng.loadFromFile("gridlr/grid_empty.png", true) && cellPng.hasAlpha() ) {
		_cellTexture = graphicsDevice()->createTexture2D(cellPng.getWidth(), cellPng.getHeight(), ciri::TextureFormat::RGBA32_UINT, 0, cellPng.getPixels());
		const auto& vp = graphicsDevice()->getViewport();
		_gridOffset.x = static_cast<float>((vp.width()  / 2) - (_cellTexture->getWidth()  * _grid->width()  / 2));
		_gridOffset.y = static_cast<float>((vp.height() / 2) - (_cellTexture->getHeight() * _grid->height() / 2));
	}


	// DEBUG - load dummy red texture
	const unsigned int RED_SIZE = 32;
	std::array<float, RED_SIZE*RED_SIZE> RED_DATA = {};
	for( int i = 0; i < RED_SIZE; ++i ) {
		for( int j = 0; j < RED_SIZE; ++j ) {
			RED_DATA[j * RED_SIZE + i] = i % 2 == 0 ? 1.0f : 0.0f;
			//RED_DATA[j * RED_SIZE + i] = cc::math::Random<float, int>::rangedReal(0, 255);
		}
	}
	//RED_DATA.fill	(255);
	_redTextureTest = graphicsDevice()->createTexture2D(RED_SIZE, RED_SIZE, ciri::TextureFormat::R32_FLOAT, 0, RED_DATA.data());
	_redSampler = graphicsDevice()->createSamplerState(ciri::SamplerDesc());
	// DEBUG - load dummy red shader
	_redShader = graphicsDevice()->createShader();
	_redShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Position, 0));
	std::string RED_VS;
	std::string RED_GS;
	std::string RED_PS;
	if( graphicsDevice()->getApiType() == ciri::GraphicsApiType::DirectX11 ) {
	RED_VS = "struct VertexOut {\n"
					 "	float2 pos : POSITION;\n"
					 "};\n"
					 "VertexOut main( float2 in_position : POSITION ) {\n"
					 "	VertexOut OUT;\n"
					 "	OUT.pos = in_position;\n"
					 "	return OUT;\n"
					 "}\n";
	RED_GS = "struct VertexOut {\n"
					 "	float2 pos : POSITION;\n"
					 "};\n"
					 "struct GeoOut {\n"
					 "	float4 hpos : SV_POSITION;\n"
					 "	float2 tex : TEXCOORD;\n"
					 "};\n"
					 "[maxvertexcount(4)]\n"
					 "void main( point VertexOut gin[1], inout TriangleStream<GeoOut> gout ) {\n"
					 "	GeoOut output;\n"
					 "	float2 pos = gin[0].pos;\n"
					 "	float size = 0.1;\n"
					 "	output.hpos = float4(pos - float2(size, size), 0.0, 1.0);\n"
					 "	output.tex = float2(0.0, 0.0);\n"
					 "	gout.Append(output);\n"
					 "	output.hpos = float4(pos + float2(-size, size), 0.0, 1.0);\n"
					 "	output.tex = float2(0.0, 1.0);\n"
					 "	gout.Append(output);\n"
					 "	output.hpos = float4(pos + float2(size, -size), 0.0, 1.0);\n"
					 "	output.tex = float2(1.0, 0.0);\n"
					 "	gout.Append(output);\n"
					 "	output.hpos = float4(pos + float2(size, size), 0.0, 1.0);\n"
					 "	output.tex = float2(1.0, 1.0);\n"
					 "	gout.Append(output);\n"
					 "	gout.RestartStrip();\n"
					 "}\n";
	RED_PS = "Texture2D<float> TheTexture : register(t0);\n"
					 "SamplerState TheSampler : register(s0);\n"
					 "struct GeoOut {\n"
					 "	float4 hpos : SV_POSITION;\n"
					 "	float2 tex : TEXCOORD;\n"
					 "};\n"
					 "float4 main( GeoOut input ) : SV_TARGET {\n"
					 "  return float4(TheTexture.Sample(TheSampler, input.tex).r, 0.0, 0.0, 1.0);\n"
					 "  //return float4(TheTexture.Sample(TheSampler, input.tex).r, 0.0, 0.0, 1.0);\n"
					 "}\n";
	} else {
	RED_VS = "#version 440\n"
					 "layout ( location = 0 ) in vec2 in_position;\n"
					 "void main() {\n"
					 "  gl_Position = vec4(in_position, 0.0, 1.0);\n"
					 "}\n";
	RED_GS = "#version 440\n"
					 "layout(points) in;\n"
					 "layout(triangle_strip) out;\n"
					 "layout(max_vertices=4) out;\n"
					 "out vec2 go_texcoord;\n"
					 "void main() {\n"
					 "  vec2 pos = gl_in[0].gl_Position.xy;\n"
					 "  float size = 0.1;"
					 "  gl_Position = vec4(pos - vec2(size, size), 0.0, 1.0);\n"
					 "  go_texcoord = vec2(0.0, 0.0)\n;"
					 "  EmitVertex();\n"
					 "  gl_Position = vec4(pos + vec2(-size, size), 0.0, 1.0);\n"
					 "  go_texcoord = vec2(0.0, 1.0)\n;"
					 "  EmitVertex();\n"
					 "  gl_Position = vec4(pos + vec2(size, -size), 0.0, 1.0);\n"
					 "  go_texcoord = vec2(1.0, 0.0)\n;"
					 "  EmitVertex();\n"
					 "  gl_Position = vec4(pos + vec2(size, size), 0.0, 1.0f);\n"
					 "  go_texcoord = vec2(1.0, 1.0)\n;"
					 "  EmitVertex();\n"
					 "  EndPrimitive();\n"
					 "}\n";
	RED_PS = "#version 440\n"
					 "uniform sampler2D TheTexture;\n"
					 "in vec2 go_texcoord;\n"
					 "out vec4 out_color;\n"
					 "void main() {\n"
					 "  out_color = vec4(texture(TheTexture, go_texcoord).r, 0.0, 0.0, 1.0);\n"
					 "}\n";
	}
	if( ciri::failed(_redShader->loadFromMemory(RED_VS.c_str(), RED_GS.c_str(), RED_PS.c_str())) ) {
		printf("Failed to load RED shader!\n");
		for( auto err : _redShader->getErrors() ) {
			printf("Error: %s\n", err.msg.c_str());
		}
	}
	// DEBUG - load dummy vb
	_redVb = graphicsDevice()->createVertexBuffer();
	const unsigned int RED_VERTS_COUNT = 4;
	std::array<cc::Vec2f, RED_VERTS_COUNT> RED_VERTS;
	const auto& vp = graphicsDevice()->getViewport();
	for( unsigned int i = 0; i < RED_VERTS_COUNT; ++i ) {
		RED_VERTS[i].x = cc::math::Random<float, int>::rangedReal(0.0f, 1.0f);
		RED_VERTS[i].y = cc::math::Random<float, int>::rangedReal(0.0f, 1.0f);
	}
	_redVb->set(RED_VERTS.data(), sizeof(cc::Vec2f), RED_VERTS_COUNT, false);
}

void Gridlr::onEvent(const ciri::WindowEvent& evt) {
	App::onEvent(evt);
}

void Gridlr::onUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// compute which cell the mouse is under
	const int mouseCellX = (input()->mouseX() - static_cast<int>(_gridOffset.x)) / _cellTexture->getWidth();
	const int mouseCellY = (input()->mouseY() - static_cast<int>(_gridOffset.y)) / _cellTexture->getHeight();
	//printf("X: %d; Y: %d\n", mouseCellX, mouseCellY);

	if( input()->isMouseButtonDown(ciri::MouseButton::Left) && !_isDragging ) {
		_isDragging = true;
		_lastMouseCell.x = mouseCellX;
		_lastMouseCell.y = mouseCellY;
		// start drag
		auto mouseBlock = _grid->get(mouseCellX, mouseCellY);
		if( mouseBlock != nullptr ) {
			if( !_grid->startDrag(*mouseBlock) ) {
				printf("BlockGrid::startDrag() failed.\n");
			}
		}
	} else if( input()->isMouseButtonDown(ciri::MouseButton::Left) && _isDragging && (_lastMouseCell.x != mouseCellX || _lastMouseCell.y != mouseCellY) ) {
		_lastMouseCell.x = mouseCellX;
		_lastMouseCell.y = mouseCellY;

		// on drag
		auto mouseBlock = _grid->get(mouseCellX, mouseCellY);
		if( mouseBlock != nullptr ) {
			if( !_grid->dragMove(*mouseBlock) ) {
				printf("BlockGrid::dragMove() failed.\n");
			}
		}
	} else if( input()->isMouseButtonUp(ciri::MouseButton::Left) && _isDragging ) {
		_isDragging = false;

		// stop drag
		auto mouseBlock = _grid->get(mouseCellX, mouseCellY);
		if( mouseBlock != nullptr ) {
			if( !_grid->stopDrag(*mouseBlock) ) {
				printf("BlockGrid::stopDrag() failed.\n");
			}
		}
	}

	if( _grid->isComplete() ) {
		printf("Complete!\n");
	}
}

void Gridlr::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	App::onFixedUpdate(deltaTime, elapsedTime);
}

void Gridlr::onDraw() {
	App::onDraw();

	const auto device = graphicsDevice();

	// clear screen
	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	// DEBUG - draw red things
	graphicsDevice()->applyShader(_redShader);
	graphicsDevice()->setVertexBuffer(_redVb);
	graphicsDevice()->setTexture2D(0, _redTextureTest, ciri::ShaderStage::Pixel);
	graphicsDevice()->setSamplerState(0, _redSampler, ciri::ShaderStage::Pixel);
	graphicsDevice()->drawArrays(ciri::PrimitiveTopology::PointList, 4, 0);

	/*
	_spriteBatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred, nullptr);
	// draw grid
	for( int y = 0; y < _grid->height(); ++y ) {
		for( int x = 0; x < _grid->width(); ++x ) {
			auto block = _grid->get(x, y);

			// vp to invert positions
			auto& vp = device->getViewport();

			// offset and inverted positions
			cc::Vec2f position = {
				_gridOffset.x + static_cast<float>(_cellTexture->getWidth() * x),
				(vp.height() - _gridOffset.y - _cellTexture->getHeight()) - (_cellTexture->getHeight() * y) // subtract from top of screen to invert the grid so it displays right; sub cell tex height is b/c of the pivot being at the top left (need to fix this)
			};

			// pick color based on state
			cc::Vec4f color;
			switch( block->state() ) {
				case gridlr::BlockState::Empty: { color = cc::Vec4f(1.0f, 1.0f, 1.0f, 1.0); break; }
				case gridlr::BlockState::One: { color = cc::Vec4f(1.0f, 0.0f, 0.0f, 1.0); break; }
				case gridlr::BlockState::Two: { color = cc::Vec4f(0.0f, 1.0f, 0.0f, 1.0); break; }
				case gridlr::BlockState::Three: { color = cc::Vec4f(0.0f, 0.0f, 1.0f, 1.0); break; }
				default: { color = cc::Vec4f(1.0f, 0.0f, 1.0f, 1.0f); break; }
			}

			// draw
			const cc::Vec2f origin(0.0f, 0.0f);
			const cc::Vec2f scale(1.0f, 1.0f);
			_spriteBatch.draw(_cellTexture, position, 0.0f, origin, scale, 0.0f, color);
		}
	}
	_spriteBatch.end();
	*/

	// present to screen
	device->present();
}

void Gridlr::onUnloadContent() {
	App::onUnloadContent();

	// clean spritebatch
	_spriteBatch.clean();
}