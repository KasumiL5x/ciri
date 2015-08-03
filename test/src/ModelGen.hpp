#ifndef __test_modelgen__
#define __test_modelgen__

#include <ciri/gfx/IGraphicsDevice.hpp>
#include "Model.hpp"

namespace modelgen {
	// width; height; depth; u-scale; v-scale
	static Model* createCube( float w, float h, float d, float us, float vs, ciri::IGraphicsDevice* device ) {
		Model* model = new Model();
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(0.0f, 0.0f, 0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));

		model->addIndex(0);  model->addIndex(1);  model->addIndex(2);  model->addIndex(0);  model->addIndex(2);  model->addIndex(3);
		model->addIndex(4);  model->addIndex(5);  model->addIndex(6);  model->addIndex(4);  model->addIndex(6);  model->addIndex(7);
		model->addIndex(8);  model->addIndex(9);  model->addIndex(10); model->addIndex(8);  model->addIndex(10); model->addIndex(11);
		model->addIndex(12); model->addIndex(13); model->addIndex(14); model->addIndex(12); model->addIndex(14); model->addIndex(15);
		model->addIndex(16); model->addIndex(17); model->addIndex(18); model->addIndex(16); model->addIndex(18); model->addIndex(19);
		model->addIndex(20); model->addIndex(21); model->addIndex(22); model->addIndex(20); model->addIndex(22); model->addIndex(23);

		if( !model->build(device) ) {
			delete model; model = nullptr;
			return nullptr;
		}
		return model;
	}
}

#endif /* __test_modelgen__ */
