#include "ClippingDemo.hpp"
#include "../../common/ModelGen.hpp"
#include <cc/MatrixFunc.hpp>
#include "ClipMesh.hpp"
#include "NewClipMesh.hpp"
#include "gt/PartitionMesh.h"
#include "gt/ConvexPolyhedron.h"

ClippingDemo::ClippingDemo()
	: Game(), _model(nullptr) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Clipping Demo";
}

ClippingDemo::~ClippingDemo() {
}

void* ClippingDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16);
}

void ClippingDemo::operator delete( void* p ) {
	_mm_free(p);
}

void ClippingDemo::onInitialize() {
	Game::onInitialize();

	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());
	
	// configure camera
	_camera.setAspect((float)window()->getWidth() / (float)window()->getHeight());
	_camera.setPlanes(0.1f, 1000.0f);
	_camera.setYaw(300.0f);
	_camera.setPitch(18.f);
	_camera.setOffset(6.0f);
	_camera.setSensitivity(1.0f, 1.0f, 0.25f);
	_camera.setLerpStrength(10.0f);
	_camera.setTarget(cc::Vec3f(0.0f, 0.0f, 0.0f));
	_camera.resetPosition();

	// configure the cutting plane
	_cuttingPlane.normal = cc::Vec3f(0.0f, 1.0f, 0.0f).normalized();
	_cuttingPlane.distance = 0.0f;
}

void ClippingDemo::onLoadContent() {
	Game::onLoadContent();

	// create grid
	if( !_grid.create(graphicsDevice()) ) {
		printf("Failed to create grid.\n");
	}

	// create axis
	if( !_axis.create(5.0f, graphicsDevice()) ) {
		printf("Failed to create axis.\n");
	}

	// create depth stencil state
	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);
	if( nullptr == _depthStencilState ) {
		printf("Failed to create depth stencil state.\n");
	}

	// create rasterizer state
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::None;
	rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterizerState = graphicsDevice()->createRasterizerState(rasterDesc);
	if( nullptr == _rasterizerState ) {
		printf("Failed to create rasterizer state.\n");
	}

	// create simple shader
	if( !_simpleShader.create(graphicsDevice()) ) {
		printf("Failed to create simple shader!\n");
	}

	// create blend state
	_blendState = graphicsDevice()->getDefaultBlendOpaque();

	// create model
	//_model = modelgen::createCube(graphicsDevice(), 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, true);
	//_model = modelgen::createPlane(graphicsDevice(), 1.0f, 1.0f, 0, 0, 1.0f, 1.0f, false, false);
	_model = new Model();
	_model->addFromObj("clipping/Plane.obj");
	_model->addIndex(0);_model->addIndex(1);_model->addIndex(2);
	_model->addIndex(3);_model->addIndex(4);_model->addIndex(5);
	_model->build(graphicsDevice());
	// clip duplicated model against cutting plane
	//_clippedModel = clipModelAgainstPlane(*_model, _cuttingPlane);

#if 0
	// build vertices for clipping
	std::vector<APoint> clipVertices;
	for( int i = 0; i < _model->getVertices().size(); ++i ) {
		const Vertex& currVert = _model->getVertices()[i];
		clipVertices.push_back(APoint(currVert.position.x, currVert.position.y, currVert.position.z));
	}
	// build indices for clipping
	std::vector<int> clipIndices;
	for( int i = 0; i < _model->getIndices().size(); ++i ) {
		clipIndices.push_back(_model->getIndices()[i]);
	}
	// convert cutting plane
	HPlane hClipPlane(_cuttingPlane.normal.x, _cuttingPlane.normal.y, _cuttingPlane.normal.z, _cuttingPlane.distance);
	// output points
	std::vector<APoint> outClipVertices;
	std::vector<int> outClipIndicesNeg;
	std::vector<int> outClipIndicesPos;
	PartitionMesh(clipVertices, clipIndices, hClipPlane, outClipVertices, outClipIndicesNeg, outClipIndicesPos);
	// rebuild only up side
	for( int i = 0; i < outClipIndicesPos.size(); ++i ) {
		const APoint& currPoint = outClipVertices[outClipIndicesPos[i]];
		_clippedModel.addVertex(Vertex(cc::Vec3f(currPoint.X(), currPoint.Y(), currPoint.Z()), cc::Vec3f(), cc::Vec2f()));
	}
	for( int i = 0; i < outClipIndicesPos.size(); ++i ) {
		_clippedModel.addIndex(i);
	}
	_clippedModel.computeNormals();
	if( !_clippedModel.build(graphicsDevice()) ) {
		printf("Failed to build clipped model.\n");
	}
	//// rebuild clipped models
	//for( int i = 0; i < outClipVertices.size(); ++i ) {
	//	const cc::Vec3f pos(outClipVertices[i].X(), outClipVertices[i].Y(), outClipVertices[i].Z());
	//	_clippedModel.addVertex(Vertex(pos, cc::Vec3f(0.0f), cc::Vec2f(0.0f)));
	//	_clippedModel2.addVertex(Vertex(pos, cc::Vec3f(0.0f), cc::Vec2f(0.0f)));
	//}
	//for( int i = 0; i < outClipIndicesNeg.size(); ++i ) {
	//	_clippedModel.addIndex(outClipIndicesNeg[i]);
	//}
	//for( int i = 0; i< outClipIndicesPos.size(); ++i ) {
	//	_clippedModel2.addIndex(outClipIndicesPos[i]);
	//}
	//if( !_clippedModel.build(graphicsDevice()) || !_clippedModel2.build(graphicsDevice()) ) {
	//	printf("Failed to build clipped model.\n");
	//}
#endif

#if 0
	std::vector<Wm5::Vector3f> clipPoints;
	std::vector<int> clipIndices;
	for( int i = 0; i < _model->getVertices().size(); ++i ) {
		const Vertex& currVert = _model->getVertices()[i];
		clipPoints.push_back(Wm5::Vector3f(currVert.position.x, currVert.position.y, currVert.position.z));
	}
	for( int i = 0; i < _model->getIndices().size(); ++i ) {
		clipIndices.push_back(_model->getIndices()[i]);
	}
	ConvexPolyhedronf clipPoly(clipPoints, clipIndices);
	ConvexPolyhedronf clipIntersection;
	Wm5::Plane3f clipPlane(Wm5::Vector3f(_cuttingPlane.normal.x, _cuttingPlane.normal.y, _cuttingPlane.normal.z), _cuttingPlane.distance);
 	clipPoly.Clip(clipPlane, clipIntersection);
	for( int i = 0; i < clipIntersection.GetNumVertices(); ++i ) {
		const Wm5::Vector3f& v = clipIntersection.GetPoint(i);
		_clippedModel.addVertex(Vertex(cc::Vec3f(v.X(), v.Y(), v.Z()), cc::Vec3f(), cc::Vec2f()));
	}
	for( int i = 0; i < clipIntersection.GetNumTriangles(); ++i ) {
		for( int j = 0; j < 3; ++j ) {
			_clippedModel.addIndex(clipIntersection.GetTriangle(i).GetVertex(j));
		}
	}
	if( !_clippedModel.build(graphicsDevice()) ) {
		printf("Failed to build clipped model.\n");
	}
#endif

	bool killme=true;

	// create clip mesh
	NewClipMesh cm(*_model);
	const int result = cm.clip(_cuttingPlane);
	printf("Cutting complete with result: %d\n", result);
	cm.printDebug(false);
	_clippedModel = cm.convert();
	_clippedModel.computeNormals();
	printf("Vertices (%d):\n", _clippedModel.getVertices().size());
	for( int i = 0 ; i < _clippedModel.getVertices().size(); ++i ) {
	}
	printf("Indices (%d):\n", _clippedModel.getIndices().size());
	for( int i = 0; i < _clippedModel.getIndices().size(); ++i ) {
		//printf("\t[%d]: %d\n", i, _clippedModel.getIndices()[i]);
	}
	if( !_clippedModel.build(graphicsDevice()) ) {
		printf("Failed to build clipped model.\n");
	}
}

void ClippingDemo::onEvent( const ciri::WindowEvent& evt ) {
	Game::onEvent(evt);

	switch( evt.type ) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::ErrorCode::CIRI_OK ) {
				printf("Failed to resize backbuffer.\n");
			}
			break;
		}
	}
}

void ClippingDemo::onUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// debug print camera information
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

	// camera movement
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::LAlt) ) {
		// rotation
		if( input()->isMouseButtonDown(ciri::MouseButton::Left) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.rotateYaw(-dx);
			_camera.rotatePitch(-dy);
		}
		// dolly
		if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
			_camera.dolly(val);
		}
		// pan
		if( input()->isMouseButtonDown(ciri::MouseButton::Middle) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.pan(dx, -dy);
		}
	}
}

void ClippingDemo::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onFixedUpdate(deltaTime, elapsedTime);

	// update camera
	_camera.update((float)deltaTime);
}

void ClippingDemo::onDraw() {
	Game::onDraw();

	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	// camera's viewproj
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// clear backbuffer
	device->setClearColor(1.0f, 1.0f, 1.0f, 1.0f);//0.05f, 0.05f, 0.05f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	// set default states
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);
	device->setBlendState(_blendState);

	//// render grid
	//if( _grid.isValid() ) {
	//	const cc::Mat4f gridXform = cameraViewProj * cc::Mat4f(1.0f);
	//	if( _grid.updateConstants(gridXform) ) {
	//		device->applyShader(_grid.getShader());
	//		device->setVertexBuffer(_grid.getVertexBuffer());
	//		device->drawArrays(ciri::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
	//	}
	//}

	// render axis
	//if( _axis.isValid() ) {
	//	const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
	//	if( _axis.updateConstants(axisXform) ) {
	//		device->applyShader(_axis.getShader());
	//		device->setVertexBuffer(_axis.getVertexBuffer());
	//		device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
	//	}
	//}

	// render cube
	if( _simpleShader.getShader() != nullptr && _model->isValid() ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = cc::Mat4f(1.0f);
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 1.0f, 1.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_model->getVertexBuffer());
			if( _model->getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_model->getIndexBuffer());
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _model->getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _model->getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}

	// render clipped cube (moved up)
	if( _simpleShader.getShader() != nullptr && _clippedModel.isValid() ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = cc::math::translate<float>(cc::Vec3f(0.0f, 1.5f, 0.0f));
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(0.0f, 1.0f, 0.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_clippedModel.getVertexBuffer());
			if( _clippedModel.getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_clippedModel.getIndexBuffer());
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _clippedModel.getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _clippedModel.getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}
	// and the second
	if( _simpleShader.getShader() != nullptr && _clippedModel2.isValid() ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = cc::math::translate<float>(cc::Vec3f(0.0f, 1.5f, 0.0f));
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 0.0f, 0.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_clippedModel2.getVertexBuffer());
			if( _clippedModel2.getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_clippedModel2.getIndexBuffer());
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _clippedModel2.getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _clippedModel2.getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}

	// present backbuffer to screen
	device->present();
}

void ClippingDemo::onUnloadContent() {
	if( _model != nullptr ) {
		delete _model;
		_model = nullptr;
	}

	// clean grid
	_grid.clean();

	// clean axis
	_axis.clean();
}

Model ClippingDemo::clipModelAgainstPlane( Model& source, const Plane& plane ) {
	Model out;

	std::vector<Model::Triangle> triangles = source.getTriangles();
	// for each polygon...
	for( Model::Triangle& currTri : triangles ) {
		// output initialized to the starting polygon (current triangle)
		std::vector<Vertex> output;
		for( int i = 0; i < 3; ++i ) {
			output.push_back(source.getVertices()[currTri.idx[i]]);
		}

		// input vector initialized to output
		std::vector<Vertex> input = output;

		// clear output
		output.clear();

		// get the starting point (last element in input list)
		Vertex& startingPoint = input[input.size()-1];

		// for all points in the input...
		//for( const Vertex& endPoint : input ) {
		for( int i = 0; i < input.size(); ++i ) {
			const Vertex& endPoint = input[i];
			// https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
			if( isPointInFrontOfPlane(_cuttingPlane, endPoint.position) ) {
				if( !isPointInFrontOfPlane(_cuttingPlane, startingPoint.position) ) {
					float t; cc::Vec3f p;
					if( intersectSegmentPlane(startingPoint.position, endPoint.position, _cuttingPlane, t, p) ) {
					Vertex v(p, cc::Vec3f::up(), cc::Vec2f::zero());
					output.push_back(v);
					}
				}
				output.push_back(endPoint);
			} else if( isPointInFrontOfPlane(_cuttingPlane, startingPoint.position) ) {
					float t; cc::Vec3f p;
					if( intersectSegmentPlane(startingPoint.position, endPoint.position, _cuttingPlane, t, p) ) {
					Vertex v(p, cc::Vec3f::up(), cc::Vec2f::zero());
					output.push_back(v);
					}
			}

			// http://gamedevelopment.tutsplus.com/tutorials/understanding-sutherland-hodgman-clipping-for-physics-engines--gamedev-11917
			//if( isPointInFrontOfPlane(_cuttingPlane, startingPoint.position) && isPointInFrontOfPlane(_cuttingPlane, endPoint.position) ) {
			//	output.push_back(endPoint);
			//} else if( isPointInFrontOfPlane(_cuttingPlane, startingPoint.position) && !isPointInFrontOfPlane(_cuttingPlane, endPoint.position) ) {
			//	float t; cc::Vec3f p;
			//	if( intersectSegmentPlane(startingPoint.position, endPoint.position, _cuttingPlane, t, p) ) {
			//	Vertex v(p, cc::Vec3f::up(), cc::Vec2f::zero());
			//	output.push_back(v);
			//	}
			//} else if( !isPointInFrontOfPlane(_cuttingPlane, startingPoint.position) && isPointInFrontOfPlane(_cuttingPlane, endPoint.position) ) {
			//	float t; cc::Vec3f p;
			//	if( intersectSegmentPlane(startingPoint.position, endPoint.position, _cuttingPlane, t, p) ) {
			//	Vertex v(p, cc::Vec3f::up(), cc::Vec2f::zero());
			//	output.push_back(v);
			//	}

			//	output.push_back(endPoint);
			//}
			startingPoint = endPoint;
			//endPoint = startingPoint;
		}

		printf("Adding %d vertices\n", output.size());
		for( const auto& curr : output ) {
			printf("\t(%f, %f, %f)\n", curr.position.x, curr.position.y, curr.position.z);
			out.addVertex(curr);
		}

		//todo: clip
		// todo: add resulting polygon to out model
	}

	out.build(graphicsDevice());

	return out;
}