#pragma once

#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Heightmap.h>
#include <lucid/gigl/Sphere.h>
#include <rnd/Utility.h>

LUCID_GAL_BEGIN

class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Mesh;

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
	enum { INSTANCE_MAXIMUM = 1024 };

	struct Instance
	{
		uint32_t                      id = 0;
		LUCID_GAL::Vector3      position;
		float32_t                  scale = 0.f;
		LUCID_GAL::Quaternion   rotation;
		LUCID_GAL::Color           color;
		LUCID_GAL::Vector4    parameters;
	};

	bool _initialized = false; // no state classes (yet)

	bool _rotating = false;
	point2d_t _cursorLocation;

	LUCID_GAL::Vector3 _viewPosition;
	LUCID_GAL::Vector3 _viewFocus;

	LUCID_GIGL::Context _context;

	LUCID_GIGL::Mesh *_hemisphere = nullptr;
	LUCID_GAL::VertexBuffer *_hemisphereInstances = nullptr;

	LUCID_GIGL::Mesh *_orbit = nullptr;
	LUCID_GAL::VertexBuffer *_orbitInstances = nullptr;

	LUCID_GIGL::Heightmap _heightmap;
	LUCID_GIGL::Sphere _sphere;

};
