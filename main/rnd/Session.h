#pragma once

#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Heightmap.h>
#include <rnd/Utility.h>

LUCID_GAL_BEGIN

class VertexFormat;
class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Material;

LUCID_GIGL_END

class Session
{
public:
	Session() = default;

	virtual ~Session() = default;

	void initialize();

	void shutdown();

	void onMouseMiddleButton(bool btnDown, point2d_t const &point);

	void onMouseWheel(int32_t delta);

	void onMouseMove(point2d_t const &point);

	void update(float64_t t, float64_t dt);

	void render(float64_t t, float32_t interpolant);

private:
	struct Instance
	{
		uint32_t                      id = 0;
		LUCID_GAL::Vector4      position;
		LUCID_GAL::Vector4      rotation;
		LUCID_GAL::Vector4 lightPosition;
		LUCID_GAL::Vector4   compositing;
		LUCID_GAL::Vector4      channel0;
		LUCID_GAL::Vector4      channel1;
		LUCID_GAL::Vector4      channel2;
	};

	bool _initialized = false; // no state classes (yet)

	bool _rotating = false;
	point2d_t _cursorLocation;

	float32_t const _znear = 10.f;
	float32_t const _zfar = 1000.f;
	float32_t const _zmid = 0.5f * (_znear + _zfar);

	LUCID_GAL::Vector3 _lightPosition;
	LUCID_GAL::Vector3 _viewPosition;
	LUCID_GAL::Vector3 _viewFocus;

	LUCID_GIGL::Context _context;

	LUCID_GAL::VertexBuffer *_instances = nullptr;

	float32_t const _radius = 75.f;

	LUCID_GIGL::Material *_material = nullptr;

	/// test {
	uint32_t _vertexCount = 0;
	uint32_t _indexCount = 0;

	LUCID_GAL::VertexFormat *_format = nullptr;
	LUCID_GAL::VertexBuffer *_vertices = nullptr;
	LUCID_GAL::IndexBuffer *_indices = nullptr;
	/// } test
};
