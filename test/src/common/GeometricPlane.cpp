#include "GeometricPlane.hpp"

namespace core = ciri::core;
namespace gfx = ciri::graphics;

GeometricPlane::GeometricPlane() {
}

GeometricPlane::GeometricPlane( const cc::Vec3f& p0, const cc::Vec3f& p1, const cc::Vec3f& p2, const cc::Vec3f& p3 ) {
	// set positions
	_verts[0].position = p0;
	_verts[1].position = p1;
	_verts[2].position = p2;
	_verts[3].position = p3;

	// compute and set normal
	const cc::Vec3f normal = getNormal();
	_verts[0].normal = _verts[1].normal = _verts[2].normal = _verts[3].normal = normal;
}

cc::Vec3f GeometricPlane::getNormal() {
	const cc::Vec3f p0 = (_xform.getWorld() * cc::Vec4f(_verts[0].position, 1.0f)).truncated();
	const cc::Vec3f p1 = (_xform.getWorld() * cc::Vec4f(_verts[1].position, 1.0f)).truncated();
	const cc::Vec3f p2 = (_xform.getWorld() * cc::Vec4f(_verts[2].position, 1.0f)).truncated();
	const cc::Vec3f p3 = (_xform.getWorld() * cc::Vec4f(_verts[3].position, 1.0f)).truncated();
	const cc::Vec3f a = p1 - p0;
	const cc::Vec3f b = p3 - p0;
	return a.cross(b).normalized();
}

cc::Vec3f GeometricPlane::getCenter() {
	const cc::Vec3f p0 = (_xform.getWorld() * cc::Vec4f(_verts[0].position, 1.0f)).truncated();
	const cc::Vec3f p1 = (_xform.getWorld() * cc::Vec4f(_verts[1].position, 1.0f)).truncated();
	const cc::Vec3f p2 = (_xform.getWorld() * cc::Vec4f(_verts[2].position, 1.0f)).truncated();
	const cc::Vec3f p3 = (_xform.getWorld() * cc::Vec4f(_verts[3].position, 1.0f)).truncated();
	return (p0 + p1 + p2 + p3) * 0.25f;
}

float GeometricPlane::getConstant() {
	return getNormal().dot(getCenter());
}

Transform& GeometricPlane::getXform() {
	return _xform;
}

bool GeometricPlane::build( std::shared_ptr<gfx::IGraphicsDevice>& device ) {
	_vb = device->createVertexBuffer();
	if( core::failed(_vb->set(_verts, sizeof(Vertex), 4, false)) ) {
		_vb->destroy();
		_vb = nullptr;
		return false;
	}

	int indices[6] = {0, 1, 2, 2, 0, 3};
	_ib = device->createIndexBuffer();
	if( core::failed(_ib->set(indices, 6, false)) ) {
		_vb->destroy();
		_vb = nullptr;
		_ib->destroy();
		_ib = nullptr;
		return false;
	}

	return true;
}


std::shared_ptr<gfx::IVertexBuffer> GeometricPlane::getVertexBuffer() const {
	return _vb;
}

std::shared_ptr<gfx::IIndexBuffer> GeometricPlane::getIndexBuffer() const {
	return _ib;
}