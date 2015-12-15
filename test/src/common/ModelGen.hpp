#ifndef __test_modelgen__
#define __test_modelgen__

#include <vector>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include "Model.hpp"

namespace modelgen {
	// http://stackoverflow.com/questions/4405787/generating-vertices-for-a-sphere
	static Model* createSphere( std::shared_ptr<ciri::IGraphicsDevice> device, int resolution, float scale ) {
		Model* model = new Model();

		const float PI = 3.14159265359f;
		std::vector<cc::Vec3f> positions;

		for( int j = 0; j < resolution; ++j ) {
			const float theta = (PI * j) / resolution;
			for( int i = 0; i < resolution; ++i ) {
				const int pos = j * resolution + i;
				const float phi = (2 * PI * i) / resolution;
				const float x = sinf(theta) * cosf(phi);
				const float y = sinf(theta) * sinf(phi);
				const float z = cosf(theta);
				positions.emplace_back(cc::Vec3f(x*scale, y*scale, z*scale));
			}
		}

		int next = 0;
		for( int j = 0; j < resolution; ++j ) {
			for( int i = 0; i < resolution; ++i ) {
				next = ((resolution-1) == i) ? 0 : i+1;
				const int pos = (j * resolution * 6) + (i * 6);

				Vertex vtx0;
				vtx0.position = positions[j * resolution + i];
				vtx0.normal = vtx0.position.normalized();
				model->addVertex(vtx0);

				Vertex vtx1;
				vtx1.position = positions[j * resolution + next];
				vtx1.normal = vtx1.position.normalized();
				model->addVertex(vtx1);

				if( (resolution-1) != j ) {
					Vertex vtx2;
					vtx2.position = positions[((j+1) * resolution) + i];
					vtx2.normal = vtx2.position.normalized();
					model->addVertex(vtx2);
				} else {
					Vertex vtx2;
					vtx2.position = cc::Vec3f(0.0f, 0.0f, -1.0f*scale);
					vtx2.normal = vtx2.position.normalized();
					model->addVertex(vtx2);
				}

				model->addVertex(model->getVertices()[pos + 2]);
				model->addVertex(model->getVertices()[pos + 1]);
				if( (resolution-1) != j ) {
					Vertex vtx3;
					vtx3.position = positions[((j+1) * resolution) + next];
					vtx3.normal = vtx3.position.normalized();
					model->addVertex(vtx3);
				} else {
					Vertex vtx3;
					vtx3.position = cc::Vec3f(0.0f, 0.0f, -1.0f*scale);
					vtx3.normal = vtx3.position.normalized();
					model->addVertex(vtx3);
				}
			}
		}

		if( !model->build(device) ) {
			delete model; model = nullptr;
			return nullptr;
		}
		return model;
	}

	// width; height; depth; u-scale; v-scale
	static Model* createCube( std::shared_ptr<ciri::IGraphicsDevice> device, float w, float h, float d, float us, float vs, bool ccw=true ) {
		Model* model = new Model();

		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f( 1.0f,  0.0f,  0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f( 1.0f,  0.0f,  0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f( 1.0f,  0.0f,  0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f( 1.0f,  0.0f,  0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(-1.0f,  0.0f,  0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(-1.0f,  0.0f,  0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f(-1.0f,  0.0f,  0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f(-1.0f,  0.0f,  0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f( 0.0f,  1.0f,  0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f( 0.0f,  1.0f,  0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f( 0.0f,  1.0f,  0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f( 0.0f,  1.0f,  0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f( 0.0f, -1.0f,  0.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f( 0.0f, -1.0f,  0.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f( 0.0f, -1.0f,  0.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f( 0.0f, -1.0f,  0.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f( 0.0f,  0.0f,  1.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f( 0.0f,  0.0f,  1.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f( 0.0f,  0.0f,  1.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f( 0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f( 0.0f,  0.0f,  1.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h, -0.5f*d), cc::Vec3f( 0.0f,  0.0f, -1.0f), cc::Vec2f(0.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w, -0.5f*h,  0.5f*d), cc::Vec3f( 0.0f,  0.0f, -1.0f), cc::Vec2f(1.0f * us,  0.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h,  0.5f*d), cc::Vec3f( 0.0f,  0.0f, -1.0f), cc::Vec2f(1.0f * us,  1.0f * vs)));
		model->addVertex(Vertex(cc::Vec3f(-0.5f*w,  0.5f*h, -0.5f*d), cc::Vec3f( 0.0f,  0.0f, -1.0f), cc::Vec2f(0.0f * us,  1.0f * vs)));

		if( ccw ) {
			model->addIndex(0);  model->addIndex(1);  model->addIndex(2);  model->addIndex(0);  model->addIndex(2);  model->addIndex(3);  // +x (right)
			model->addIndex(4);  model->addIndex(5);  model->addIndex(6);  model->addIndex(4);  model->addIndex(6);  model->addIndex(7);  // -x (left)
			model->addIndex(8);  model->addIndex(9);  model->addIndex(10); model->addIndex(8);  model->addIndex(10); model->addIndex(11); // +y (top)
			model->addIndex(12); model->addIndex(13); model->addIndex(14); model->addIndex(12); model->addIndex(14); model->addIndex(15); // -y (bottom)
			model->addIndex(16); model->addIndex(17); model->addIndex(18); model->addIndex(16); model->addIndex(18); model->addIndex(19); // +z (front)
			model->addIndex(20); model->addIndex(21); model->addIndex(22); model->addIndex(20); model->addIndex(22); model->addIndex(23); // -z (back)
		} else {
			model->addIndex(2);  model->addIndex(1);  model->addIndex(0);  model->addIndex(3);  model->addIndex(2);  model->addIndex(0);  // +x (right)
			model->addIndex(6);  model->addIndex(5);  model->addIndex(4);  model->addIndex(7);  model->addIndex(6);  model->addIndex(4);  // -x (left)
			model->addIndex(10);  model->addIndex(9);  model->addIndex(8); model->addIndex(11);  model->addIndex(10); model->addIndex(8); // +y (top)
			model->addIndex(14); model->addIndex(13); model->addIndex(12); model->addIndex(15); model->addIndex(14); model->addIndex(12); // -y (bottom)
			model->addIndex(18); model->addIndex(17); model->addIndex(16); model->addIndex(19); model->addIndex(18); model->addIndex(16); // +z (front)
			model->addIndex(22); model->addIndex(21); model->addIndex(20); model->addIndex(23); model->addIndex(22); model->addIndex(20); // -z (back)
		}

		if( !model->build(device) ) {
			delete model; model = nullptr;
			return nullptr;
		}
		return model;
	}
	
	static Model* createFullscreenQuad( std::shared_ptr<ciri::IGraphicsDevice> device ) {
		Model* model = new Model();
		//model->addVertex(Vertex(cc::Vec3f(-1.0f, -1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		//model->addVertex(Vertex(cc::Vec3f( 1.0f, -1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		//model->addVertex(Vertex(cc::Vec3f(-1.0f,  1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		//model->addVertex(Vertex(cc::Vec3f(-1.0f,  1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		//model->addVertex(Vertex(cc::Vec3f( 1.0f, -1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		//model->addVertex(Vertex(cc::Vec3f( 1.0f,  1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));

		model->addVertex(Vertex(cc::Vec3f(-1.0f, -1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		model->addVertex(Vertex(cc::Vec3f( 1.0f, -1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		model->addVertex(Vertex(cc::Vec3f( 1.0f,  1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));
		model->addVertex(Vertex(cc::Vec3f(-1.0f,  1.0f, 0.0f), cc::Vec3f(), cc::Vec2f()));

		model->addIndex(0); model->addIndex(1); model->addIndex(3);
		model->addIndex(3); model->addIndex(1); model->addIndex(2);

		if( !model->build(device) ) {
			delete model; model = nullptr;
			return nullptr;
		}
		return model;
	}

	static Model* createPlane( std::shared_ptr<ciri::IGraphicsDevice> device, float width, float height, int divsX=0, int divsY=0, float uScale=1.0f, float vScale=1.0f, bool dynamicVertex=false, bool dynamicIndex=false ) {
		// add 1 such that asking for 1 division will add a split in the middle; asking for 0 returns just a quad
		divsX += 1;
		divsY += 1;

		if( width <= 0.0f || height <= 0.0f || divsX < 1 || divsY < 1 ) {
			return nullptr;
		}

		Model* model = new Model();
		model->setDynamicity(dynamicVertex, dynamicIndex);

		// create and set vertices
		const int u = divsX+1;
		const int v = divsY+1;
		for( int j = 0; j <= divsY; ++j ) {
			for( int i = 0; i <= divsX; ++i ) {
				const float x = ((float(i) / (u-1)) * 2.0f - 1.0f) * width * 0.5f;
				const float y = 0.0f;
				const float z = ((float(j) / (v-1)) * 2.0f - 1.0f) * height * 0.5f;

				const float tx = (static_cast<float>(i) / static_cast<float>(divsX)) * uScale;
				const float ty = (static_cast<float>(j) / static_cast<float>(divsY)) * vScale;

				model->addVertex(Vertex(cc::Vec3f(x, y, z), cc::Vec3f::up(), cc::Vec2f(tx, ty)));
			}
		}

		// create and set indices
		const int indexCount = divsX * divsY * 2 * 3;
		std::vector<int> indices(indexCount);
		int* id = &indices[0];
		for( int i = 0; i < divsY; ++i ) {
			for( int j = 0; j < divsX; ++j ) {
				const int i0 = i * (divsX+1) + j;
				const int i1 = i0 + 1;
				const int i2 = i0 + (divsX+1);
				const int i3 = i2 + 1;
				if( (j+2) % 2 ) {
					*id++= i0; *id++= i2; *id++= i1;
					*id++= i1; *id++= i2; *id++= i3;
				} else {
					*id++= i0; *id++= i2; *id++= i3;
					*id++= i0; *id++= i3; *id++= i1;
				}
			}
		}

		for( int i = 0; i < indexCount; ++i ) {
			model->addIndex(indices[i]);
		}

		model->computeTangents();

		if( !model->build(device) ) {
			delete model; model = nullptr;
			return nullptr;
		}
		return model;
	}
}

#endif /* __test_modelgen__ */
