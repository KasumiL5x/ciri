#ifndef __test_modelgen__
#define __test_modelgen__

#include <vector>
#include <ciri/Graphics.hpp>
#include "Model.hpp"

namespace modelgen {
	// http://stackoverflow.com/questions/4405787/generating-vertices-for-a-sphere
	static Model* createSphere( std::shared_ptr<ciri::IGraphicsDevice> device, int resolution, float scale ) {
		// todo: generate icosphere w/ uv coordinates: http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

		Model* model = new Model();

		const float PI = 3.14159265359f;
		const int width = resolution;
		const int height = resolution;
		for( int j = 1; j < (height-1); ++j ) {
			for( int i = 0; i < width; ++i ) {
				const float theta = float(j) / (height-1) * PI;
				const float phi = float(i) / (width-1) * PI * 2.0f;

				const float x = sinf(theta) * cosf(phi) * scale;
				const float y = cosf(theta) * scale;
				const float z = -sinf(theta) * sinf(phi) * scale;

				Vertex vtx;
				vtx.position = cc::Vec3f(x, y, z);
				vtx.normal = vtx.position.normalized();
				//vtx.texcoord = cc::Vec2f(atanf(theta) / PI, (phi) + 0.5f);
				////vtx.texcoord = cc::Vec2f(asinf(vtx.normal.x) / PI + 0.5f, asinf(vtx.normal.y) / PI + 0.5f);
				//vtx.texcoord = cc::Vec2f(atan2f(x, z), atan2f(y, sqrt(x*x+z*z)));
				model->addVertex(vtx);
			}
		}

		Vertex topVtx;
		topVtx.position = cc::Vec3f(0.0f, 1.0f * scale, 0.0f);
		topVtx.normal = topVtx.position.normalized();
		////topVtx.texcoord = cc::Vec2f(asinf(topVtx.normal.x) / PI + 0.5f, asinf(topVtx.normal.y) / PI + 0.5f);
		//topVtx.texcoord = cc::Vec2f(atan2f(topVtx.position.x, topVtx.position.z), atan2f(topVtx.position.y, sqrt(topVtx.position.x*topVtx.position.x+topVtx.position.z*topVtx.position.z)));
		model->addVertex(topVtx);

		Vertex btmVtx;
		btmVtx.position = cc::Vec3f(0.0f, -1.0f * scale, 0.0f);
		btmVtx.normal = btmVtx.position.normalized();
		////btmVtx.texcoord = cc::Vec2f(asinf(btmVtx.normal.x) / PI + 0.5f, asinf(btmVtx.normal.y) / PI + 0.5f);
		//btmVtx.texcoord = cc::Vec2f(atan2f(btmVtx.position.x, btmVtx.position.z), atan2f(btmVtx.position.y, sqrt(btmVtx.position.x*btmVtx.position.x+btmVtx.position.z*btmVtx.position.z)));
		model->addVertex(btmVtx);

		for( unsigned int i = 0; i < model->getVertices().size(); ++i ) {
			const cc::Vec3f pos = model->getVertices()[i].position / scale;
			const cc::Vec3f nrm = model->getVertices()[i].normal;
			//const float rn = 2.0f * sqrt(pos.x*pos.x + pos.y*pos.y + ((pos.z+1.0f)*(pos.z+1.0f)));
			const cc::Vec3f r = pos - (2.0f * nrm) * nrm * pos;
			const float rn = 2.0f * sqrtf(r.x*r.x + r.y*r.y + (r.z+1.0f)*(r.z+1.0f));
			const float u = (r.x / rn) + 0.5f;
			const float v = (r.y / rn) + 0.5f;

			//const float rn = 2.0f * sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
			//const float u = pos.x / rn + 0.5f;
			//const float v = pos.y / rn + 0.5f;

			//const float len = sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
			//const float v = acosf(pos.y / len) / PI;
			//const float u = (atan2f(pos.z, pos.x) / PI + 1.0f) * 0.5f;

			//const float theta = atan2f(pos.z, pos.x);
			//const float phi = acosf(pos.y / sqrtf(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z));
			//const float u = theta / (2.0f * PI);
			//const float v = phi / PI;

			// position-based spherical mapping
			//const float u = asinf(pos.x / scale) / PI + 0.5f;
			//const float v = asinf(pos.y / scale) / PI + 0.5f;
			
			// spherical mapping (has a seam)
			//float u = atan2f(-pos.x, pos.z);
			//float v = atanf(pos.y / sqrt(pos.x*pos.x + pos.z*pos.z));
			//u = 1.0f - u / (2.0f * PI);
			//v = fabsf(0.5f - v / PI);
			//u = u - floorf(u);
			//v = v;
			model->getVertices()[i].texcoord = cc::Vec2f(u, v);
		}

		for( int j = 0; j < (height-3); ++j ) {
			for( int i = 0; i < (width-1); ++i ) {
				model->addIndex((j  )*width + i  );
				model->addIndex((j+1)*width + i+1);
				model->addIndex((j  )*width + i+1);
				
				model->addIndex((j  )*width + i  );
				model->addIndex((j+1)*width + i  );
				model->addIndex((j+1)*width + i+1);
			}
		}
		for( int i = 0; i < (width-1); ++i ) {
			model->addIndex((height-2)*width);
			model->addIndex(i);
			model->addIndex(i+1);

			model->addIndex((height-2)*width+1);
			model->addIndex((height-3)*width + i);
			model->addIndex((height-3)*width + i+1);
		}

		//const float PI = 3.14159265359f;
		//std::vector<cc::Vec3f> positions;
		//std::vector<cc::Vec2f> texcoords;

		//for( int j = 0; j < resolution; ++j ) {
		//	const float theta = (PI * j) / resolution;
		//	for( int i = 0; i < resolution; ++i ) {
		//		const int pos = j * resolution + i;
		//		const float phi = (2 * PI * i) / resolution;
		//		const float x = sinf(theta) * cosf(phi);
		//		const float y = sinf(theta) * sinf(phi);
		//		const float z = cosf(theta);
		//		positions.emplace_back(cc::Vec3f(x*scale, y*scale, z*scale));
		//		const float U_COORD = (float(j) / float(resolution) * PI) / (2.0 * PI);
		//		const float V_COORD = (float(i) / float(resolution) * PI * 2.0) / PI;
		//		texcoords.emplace_back(cc::Vec2f(U_COORD, V_COORD));
		//	}
		//}

		//int next = 0;
		//for( int j = 0; j < resolution; ++j ) {
		//	for( int i = 0; i < resolution; ++i ) {
		//		next = ((resolution-1) == i) ? 0 : i+1;
		//		const int pos = (j * resolution * 6) + (i * 6);

		//		Vertex vtx0;
		//		vtx0.position = positions[j * resolution + i];
		//		vtx0.normal = vtx0.position.normalized();
		//		vtx0.texcoord = texcoords[j * resolution + i];
		//		model->addVertex(vtx0);

		//		Vertex vtx1;
		//		vtx1.position = positions[j * resolution + next];
		//		vtx1.normal = vtx1.position.normalized();
		//		vtx1.texcoord = texcoords[j * resolution + next];
		//		model->addVertex(vtx1);

		//		if( (resolution-1) != j ) {
		//			Vertex vtx2;
		//			vtx2.position = positions[((j+1) * resolution) + i];
		//			vtx2.normal = vtx2.position.normalized();
		//			vtx2.texcoord = texcoords[((j+1) * resolution) + i];
		//			model->addVertex(vtx2);
		//		} else {
		//			Vertex vtx2;
		//			vtx2.position = cc::Vec3f(0.0f, 0.0f, -1.0f*scale);
		//			vtx2.normal = vtx2.position.normalized();
		//			vtx2.texcoord = cc::Vec2f(0.0f, 0.0f);
		//			model->addVertex(vtx2);
		//		}

		//		model->addVertex(model->getVertices()[pos + 2]);
		//		model->addVertex(model->getVertices()[pos + 1]);
		//		if( (resolution-1) != j ) {
		//			Vertex vtx3;
		//			vtx3.position = positions[((j+1) * resolution) + next];
		//			vtx3.normal = vtx3.position.normalized();
		//			vtx3.texcoord = texcoords[((j+1) * resolution) + next];
		//			model->addVertex(vtx3);
		//		} else {
		//			Vertex vtx3;
		//			vtx3.position = cc::Vec3f(0.0f, 0.0f, -1.0f*scale);
		//			vtx3.normal = vtx3.position.normalized();
		//			vtx3.texcoord = cc::Vec2f(0.0f, 0.0f);
		//			model->addVertex(vtx3);
		//		}
		//	}
		//}

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

		model->computeTangents();

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

				const float tx = 1.0f-(static_cast<float>(i) / static_cast<float>(divsX)) * uScale;
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
