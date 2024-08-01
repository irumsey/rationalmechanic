#include "Session.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/math/Algorithm.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/AABB.h>
#include <lucid/core/Random.h>
#include <lucid/core/Numbers.h>

#include <fstream>
#include <algorithm>

#ifdef min
#	undef min
#endif

namespace /* anonymous */ {

	typedef LUCID_MATH::AABB<float32_t, 3> Box;

	template<class T> inline void safe_delete(T *&ptr)
	{
		delete ptr;
		ptr = nullptr;
	}

	inline LUCID_GAL::Vector2 computeConicPoint(float32_t hu, float32_t ecc, float32_t theta)
	{
		float32_t c = LUCID_MATH::cos(theta);
		float32_t s = LUCID_MATH::sin(theta);

		return LUCID_GAL::Vector2(c, s) * hu / (1.f + ecc * c);
	}

}

void Session::initialize()
{
	shutdown();

	_viewPosition = LUCID_GAL::Vector3(100, 100, 100);

	_context = LUCID_GIGL::Context("content/render.context");

	_hemisphere = LUCID_GIGL::Mesh::create("content/hemisphere.mesh");
	_hemisphereInstances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, INSTANCE_MAXIMUM, sizeof(Instance));

	Instance *instances = (Instance *)(_hemisphereInstances->lock());
	for (size_t i = 0; i < 10; ++i)
	{
		Instance &instance = instances[i];

		instance.id = 0;
		instance.position = LUCID_CORE::random::real(0.f, 75.f) * LUCID_GAL::Vector3(LUCID_CORE::random::real(-1, 1), LUCID_CORE::random::real(-1, 1), LUCID_CORE::random::real(-1, 1));
		instance.rotation = LUCID_GAL::Quaternion();
		instance.scale = LUCID_CORE::random::real(2.f, 10.f);
		instance.color = LUCID_GAL::Color(0, 1, 0, 0.33f);
		instance.parameters = LUCID_GAL::Vector4();
	}
	_hemisphereInstances->unlock();

	_orbit = LUCID_GIGL::Mesh::create("content/orbit.mesh");
	_orbitInstances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, INSTANCE_MAXIMUM, sizeof(Instance));

	instances = (Instance *)(_orbitInstances->lock());
	for (size_t i = 0; i < 1; ++i)
	{
		Instance &instance = instances[i];

		float32_t   a = 100.f;
		float32_t ecc = 0.5f;
		float32_t  hu = a * (1.f - ecc * ecc);

		instance.id = 0;
		instance.position = LUCID_GAL::Vector3();
		instance.rotation = LUCID_GAL::Quaternion();
		instance.scale = 4;
		instance.color = LUCID_GAL::Color(0, 0, 1, 1);
		instance.parameters = LUCID_GAL::Vector4(hu, ecc, 0.f, LUCID_CORE_NUMBERS::two_pi<float32_t>);

	}
	_orbitInstances->unlock();

	_initialized = true;
}

void Session::shutdown()
{
	::safe_delete(_orbitInstances);
	::safe_delete(_orbit);

	::safe_delete(_hemisphereInstances);
	::safe_delete(_hemisphere);

	_initialized = false;
}

void Session::onMouseMiddleButton(bool btnDown, point2d_t const &point)
{
	if (!_initialized)
		return;

	_rotating = btnDown;
	_blah = btnDown ? point : _blah;
}

void Session::onMouseWheel(int32_t delta)
{
	if (!_initialized)
		return;

	float32_t d = LUCID_MATH::len(_viewPosition);
	_viewPosition = _viewPosition * (1.f - float32_t(delta) / (30.f * d));
}

void Session::onMouseMove(point2d_t const &point)
{
	if (!_initialized)
		return;

	if (!_rotating)
		return;

	LUCID_GAL::Matrix4x4 const &viewMatrix = _context["viewMatrix"].as<LUCID_GAL::Matrix4x4>();

	float32_t theta = 0.01f * (_blah.x - point.x);
	float32_t   phi = 0.01f * (point.y - _blah.y);
	_blah = point;

	float32_t c = LUCID_MATH::cos(theta);
	float32_t s = LUCID_MATH::sin(theta);

	LUCID_GAL::Quaternion qz = LUCID_GAL::Quaternion(0, 0, s, 0.5f * c);
	LUCID_GAL::Matrix3x3 Rz = LUCID_MATH::matrixFromQuaternion(qz);

	c = LUCID_MATH::cos(phi);
	s = LUCID_MATH::sin(phi);

	LUCID_GAL::Vector3 viewRight = LUCID_MATH::extractViewRight(viewMatrix);
	LUCID_GAL::Quaternion qx = LUCID_GAL::Quaternion(s * viewRight.x, s * viewRight.y, s * viewRight.z, 0.5f * c);
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
	float32_t const zfar = 2000.f;

	LUCID_GAL::Vector3 viewFocus;

	LUCID_GAL::Matrix4x4 viewMatrix = LUCID_MATH::look(_viewPosition, viewFocus, LUCID_GAL::Vector3(0, 0, 1));
	LUCID_GAL::Matrix4x4 projMatrix = LUCID_MATH::perspective(fov, aspect, znear, zfar);
	LUCID_GAL::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;

	_context["texelSize"] = texelSize;

	_context["lightPosition"] = LUCID_GAL::Vector3(100, -100, 100);

	_context["viewPosition"] = _viewPosition;

	_context["viewUp"] = LUCID_GAL::Vector3(viewMatrix.xx, viewMatrix.xy, viewMatrix.xz);
	_context["viewRight"] = LUCID_GAL::Vector3(viewMatrix.yx, viewMatrix.yy, viewMatrix.yz);
	_context["viewForward"] = LUCID_GAL::Vector3(-viewMatrix.zx, -viewMatrix.zy, -viewMatrix.zz);

	_context["viewMatrix"] = viewMatrix;
	_context["invViewMatrix"] = LUCID_MATH::inverse(viewMatrix);

	_context["projMatrix"] = projMatrix;
	_context["invProjMatrix"] = LUCID_MATH::inverse(projMatrix);

	_context["viewProjMatrix"] = viewProjMatrix;
	_context["invViewProjMatrix"] = LUCID_MATH::inverse(viewProjMatrix);
}

void Session::render(float64_t t, float32_t interpolant)
{
	if (!_initialized)
		return;

	_context["time"] = float32_t(t);
	_context["interpolant"] = float32_t(interpolant);

	LUCID_GAL_PIPELINE.setVertexStream(1, _hemisphereInstances);
	_hemisphere->renderInstanced(_context, 10);

	LUCID_GAL_PIPELINE.setVertexStream(1, _orbitInstances);
	_orbit->renderInstanced(_context, 1);
}
