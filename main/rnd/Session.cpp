#include "Session.h"
#include <lucid/orbit/Utility.h>
#include <lucid/gigl/ROAM.h>
#include <lucid/gigl/Material.h>
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Math.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Random.h>
#include <lucid/core/Numbers.h>

#include <fstream>
#include <algorithm>

#ifdef min
#	undef min
#endif

namespace roam = ::lucid::gigl::roam;

namespace /* anonymous */ {

	template<class T> inline void safe_delete(T*& ptr)
	{
		delete ptr;
		ptr = nullptr;
	}

	struct Vertex
	{
		LUCID_GAL::Vector3 position;
		float32_t height = 0.f;
		LUCID_GAL::Vector2 texcoord;
	};
}

void Session::initialize()
{
	shutdown();

	LUCID_GAL::Quaternion q = LUCID_MATH::rotateUsingAxis(LUCID_GAL::Vector3(1, 0, 0), 0.7f);
	LUCID_GAL::Vector3 r = LUCID_MATH::matrixFromQuaternion(q) * LUCID_GAL::Vector3(0, 1, 0);

	_lightPosition = LUCID_GAL::Vector3(-100, 100, 100);
	_viewPosition = LUCID_GAL::Vector3(100, 100, 100);
	_viewFocus = LUCID_GAL::Vector3();

	LUCID_GAL::Vector3 viewDirection = _viewPosition - _viewFocus;

	_context = LUCID_GIGL::Context("content/render.context");

	/// test {

	LUCID_GIGL::Heightmap heightmap("content/earth.heightmap", 10, 3);
	
	roam::Geometry<uint32_t> geometry;

	std::vector<Vertex> vertices = {
		{ {  1,  0,  0, }, 0, { 0.00, 0.50, } },
		{ {  0,  1,  0, }, 0, { 0.25, 0.50, } },
		{ { -1,  0,  0, }, 0, { 0.50, 0.50, } },
		{ {  0, -1,  0, }, 0, { 0.75, 0.50, } },
		{ {  1,  0,  0, }, 0, { 1.00, 0.50, } },
		{ {  0,  0,  1, }, 0, { 0.00, 1.00, } },
		{ {  0,  0, -1, }, 0, { 0.00, 0.00, } },
	};

	geometry.faces = {
		{ 0, 1, 5, },
		{ 1, 2, 5, },
		{ 2, 3, 5, },
		{ 3, 4, 5, },
		{ 4, 3, 6, },
		{ 3, 2, 6, },
		{ 2, 1, 6, },
		{ 1, 0, 6, },
	};

	roam::initialize(geometry);

	auto split_edge = [&heightmap, &geometry, &vertices](roam::Edge<uint32_t> const &edge) -> uint32_t {
		Vertex vertex;

		Vertex const &v0 = vertices[edge[0]];
		Vertex const &v1 = vertices[edge[1]];

		vertex.position = LUCID_MATH::normalize(0.5f * (v1.position + v0.position));
		
		float32_t u = LUCID_MATH::atan2(vertex.position.y, vertex.position.x);
		float32_t v = LUCID_MATH::acos(vertex.position.z);

		u = LUCID_CORE_NUMBERS::inv_two_pi<float32_t> * ((u < 0.f) ? u + LUCID_CORE_NUMBERS::two_pi<float32_t> : u );
		v = LUCID_CORE_NUMBERS::inv_pi<float32_t> * v;

		bool west = (v0.texcoord.x > 0.5f) || (v1.texcoord.x > 0.5f);
		u = ((0.f == u) && west) ? 1.f : u;

		vertex.texcoord = LUCID_GAL::Vector2(u, v);
		vertex.height = float32_t(heightmap.at(vertex.texcoord));

		uint32_t vertexIndex = uint32_t(vertices.size());
		vertices.push_back(vertex);
		return vertexIndex;
	};

	size_t first = 0;
	for (size_t depth = 0; depth < 7; ++depth)
	{
		size_t end = geometry.faces.size();
		for (size_t index = first; index < end; ++index)
			roam::split_face(geometry, split_edge, index);
		first = end;
	}

	first = 0;
	for (size_t depth = 0; depth < 7; ++depth)
	{
		size_t end = geometry.faces.size();
		for (size_t index = first; index < end; ++index)
		{
			if (geometry.not_leaf(index))
				continue;

			Vertex const v0 = vertices[geometry[index][0]];
			Vertex const v1 = vertices[geometry[index][1]];
			Vertex const v2 = vertices[geometry[index][2]];

			auto area = LUCID_MATH::fit(v0.texcoord, v1.texcoord, v2.texcoord);
			LUCID_GIGL::Heightmap::Tile const &tile = heightmap.filter(area);

			uint16_t error = tile.h[1] - tile.h[0];
			if (error > 32)
				roam::split_face(geometry, split_edge, index);
		}
		first = end;
	}

	size_t end = geometry.faces.size();
	for (size_t index = 0; index < end; ++index)
	{
		if (geometry.not_leaf(index))
			continue;

		roam::Face<uint32_t> const face = geometry[index];

		Vertex &v0 = vertices[face[0]];
		Vertex &v1 = vertices[face[1]];
		Vertex &v2 = vertices[face[2]];

		uint16_t h2 = heightmap.at(v2.texcoord);
		uint16_t h1 = heightmap.at(v1.texcoord);
		uint16_t h0 = heightmap.at(v0.texcoord);

		uint8_t code = ((h2 > 32) << 2) | ((h1 > 32) << 1) | ((h0 > 32) << 0);
		code = 0x07 & code;

		/// all land
		if (7 == code)
		{
			v0.height = 1.f;
			v1.height = 1.f;
			v2.height = 1.f;
			continue;
		}

		geometry.surface[index] = false;

		/// all water
		if (0 == code)
			continue;

#if true
		uint32_t l = split_edge(face. base());
		uint32_t m = split_edge(face.right());
		uint32_t n = split_edge(face. left());

		vertices[l].height = 0.5f;
		vertices[m].height = 0.5f;
		vertices[n].height = 0.5f;

		typedef roam::Face<uint32_t> face_t;

		switch (code)
		{
		case 1:
			geometry.faces.push_back(face_t(geometry[index][0], l, n));
			geometry.surface.push_back(true);
			break;
		case 2:
			geometry.faces.push_back(face_t(l, geometry[index][1], m));
			geometry.surface.push_back(true);
			break;
		case 3:
			geometry.faces.push_back(face_t(geometry[index][0], l, n));
			geometry.surface.push_back(true);
			geometry.faces.push_back(face_t(l, m, n));
			geometry.surface.push_back(true);
			geometry.faces.push_back(face_t(l, geometry[index][1], m));
			geometry.surface.push_back(true);
			break;
		case 4:
			geometry.faces.push_back(face_t(n, m, geometry[index][2]));
			geometry.surface.push_back(true);
			break;
		case 5:
			geometry.faces.push_back(face_t(geometry[index][0], l, n));
			geometry.surface.push_back(true);
			geometry.faces.push_back(face_t(l, m, n));
			geometry.surface.push_back(true);
			geometry.faces.push_back(face_t(n, m, geometry[index][2]));
			geometry.surface.push_back(true);
			break;
		case 6:
			geometry.faces.push_back(face_t(l, geometry[index][1], m));
			geometry.surface.push_back(true);
			geometry.faces.push_back(face_t(l, m, n));
			geometry.surface.push_back(true);
			geometry.faces.push_back(face_t(n, m, geometry[index][2]));
			geometry.surface.push_back(true);
			break;
		default:
			break;
		}
#endif
	}

    auto face = geometry.  faces.begin();
    auto leaf = geometry.surface.begin();

    while (face != geometry.faces.end())
    {
        if (*leaf)
        {
            ++face;
            ++leaf;
        }
        else
        {
            face = geometry.  faces.erase(face);
            leaf = geometry.surface.erase(leaf);
        }
    }

	_vertexCount = uint32_t(vertices.size());
	size_t faceCount = geometry.faces.size();
	_indexCount = uint32_t(3 * faceCount);

	_material = LUCID_GIGL::Material::create("content/earth.material");
	_format = LUCID_GAL::VertexFormat::create(LUCID_CORE::FileReader("content/planet.format"));
	_vertices = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_STATIC, _vertexCount, sizeof(Vertex));
	_indices = LUCID_GAL::IndexBuffer::create(LUCID_GAL::IndexBuffer::USAGE_STATIC, LUCID_GAL::IndexBuffer::FORMAT_UINT32, _indexCount);
	_instances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, 100, sizeof(Instance));

	std::memcpy(_vertices->lock(), &(vertices[0]), _vertexCount * sizeof(Vertex));
	_vertices->unlock();

	std::memcpy(_indices->lock(), &(geometry.faces[0]), faceCount * sizeof(roam::Face<uint32_t>));
	_indices->unlock();

	/// } test

	_initialized = true;
}

void Session::shutdown()
{
	::safe_delete(_instances);
	::safe_delete(_indices);
	::safe_delete(_vertices);
	::safe_delete(_format);
	::safe_delete(_material);

	_initialized = false;
}

void Session::onMouseMiddleButton(bool btnDown, point2d_t const &point)
{
	if (!_initialized)
		return;

	_rotating = btnDown;
	_cursorLocation = btnDown ? point : _cursorLocation;
}

void Session::onMouseWheel(int32_t delta)
{
	if (!_initialized)
		return;

	_viewPosition = _viewPosition + 0.01f * float32_t(delta) * LUCID_MATH::normalize(_viewFocus - _viewPosition);
}

void Session::onMouseMove(point2d_t const &point)
{
	if (!_initialized)
		return;

	if (!_rotating)
		return;

	float32_t theta = 0.01f * (_cursorLocation.x - point.x);
	float32_t   phi = 0.01f * (point.y - _cursorLocation.y);
	_cursorLocation = point;

	LUCID_GAL::Quaternion qz = LUCID_MATH::rotateUsingAxis(LUCID_GAL::Vector3(0, 0, 1), theta);
	LUCID_GAL::Matrix3x3 Rz = LUCID_MATH::matrixFromQuaternion(qz);

	LUCID_GAL::Vector3 viewForward = LUCID_MATH::normalize(_viewFocus - _viewPosition);
	LUCID_GAL::Vector3 viewRight = LUCID_MATH::normalize(LUCID_MATH::cross(viewForward, LUCID_GAL::Vector3(0, 0, 1)));
	LUCID_GAL::Quaternion qx = LUCID_MATH::rotateUsingAxis(viewRight, phi);
	LUCID_GAL::Matrix3x3 Rx = LUCID_MATH::matrixFromQuaternion(qx);

	_viewPosition = Rx * Rz * _viewPosition;
}

void Session::update(float64_t t, float64_t dt)
{
	if (!_initialized)
		return;

	float32_t width = float32_t(LUCID_GAL_SYSTEM.width());
	float32_t height = float32_t(LUCID_GAL_SYSTEM.height());
	LUCID_GAL::Vector2 const texelSize = LUCID_GAL::Vector2(1.f / width, 1.f / height);

	float32_t const fov = 0.3f * LUCID_CORE_NUMBERS::pi<float32_t>;
	float32_t const aspect = LUCID_GAL_SYSTEM.aspect();
	float32_t const znear = 10.f;
	float32_t const zfar = 1000.f;

	LUCID_GAL::Matrix4x4 viewMatrix = LUCID_MATH::look(LUCID_GAL::Vector3(), _viewFocus - _viewPosition, LUCID_GAL::Vector3(0, 0, 1));
	LUCID_GAL::Matrix4x4 projMatrix = LUCID_MATH::perspective(fov, aspect, znear, zfar);
	LUCID_GAL::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;

	_context["texelSize"] = texelSize;

	_context["lightPosition"] = _lightPosition;

	_context["viewPosition"] = LUCID_GAL::Vector3();

	_context["viewUp"] = LUCID_GAL::Vector3(viewMatrix.xx, viewMatrix.xy, viewMatrix.xz);
	_context["viewRight"] = LUCID_GAL::Vector3(viewMatrix.yx, viewMatrix.yy, viewMatrix.yz);
	_context["viewForward"] = LUCID_GAL::Vector3(-viewMatrix.zx, -viewMatrix.zy, -viewMatrix.zz);

	_context["viewMatrix"] = viewMatrix;
	_context["invViewMatrix"] = LUCID_MATH::inverse(viewMatrix);

	_context["projMatrix"] = projMatrix;
	_context["invProjMatrix"] = LUCID_MATH::inverse(projMatrix);

	_context["viewProjMatrix"] = viewProjMatrix;
	_context["invViewProjMatrix"] = LUCID_MATH::inverse(viewProjMatrix);

	Instance instance;
	
	LUCID_GAL::Vector3 position = LUCID_GAL::Vector3() - _viewPosition;
	float32_t const distance = LUCID_MATH::len(position);

	instance.id = 1;
	instance.position = _zmid * LUCID_GAL::Vector4(position / distance, _radius / distance);;
	instance.rotation = LUCID_GAL::Vector4(0, 0, 0, 1);
	instance.lightPosition = LUCID_GAL::Vector4(_zmid * _lightPosition / distance, 0);
	instance.compositing = LUCID_GAL::Vector4(_radius / distance, distance / _zmid, 0, 0);
	instance.channel0 = LUCID_GAL::Vector4();
	instance.channel1 = LUCID_GAL::Vector4();
	instance.channel2 = LUCID_GAL::Vector4();

	_instances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, 100, sizeof(Instance));
	Instance *buffer = _instances->lock_as<Instance>();
	buffer[0] = instance;
	_instances->unlock();
}

void Session::render(float64_t t, float32_t interpolant)
{
	if (!_initialized)
		return;

	_context["time"] = float32_t(t);
	_context["interpolant"] = float32_t(interpolant);

	_material->begin(_context);
		LUCID_GAL_PIPELINE.beginGeometry(_format);
			LUCID_GAL_PIPELINE.setVertexStream(0, _vertices);
			LUCID_GAL_PIPELINE.setVertexStream(1, _instances);
			LUCID_GAL_PIPELINE.setIndexStream(_indices);
			LUCID_GAL_PIPELINE.drawInstanced(LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_LIST, _vertexCount, _indexCount, 1);
		LUCID_GAL_PIPELINE.endGeometry(_format);
	_material->end();
}
