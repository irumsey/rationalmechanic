#include "Session.h"
#include <lucid/orbit/Utility.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/math/Math.h>
#include <lucid/core/Random.h>
#include <lucid/core/Numbers.h>

#include <fstream>
#include <algorithm>

#ifdef min
#	undef min
#endif

namespace /* anonymous */ {

	typedef LUCID_MATH::AABB<float32_t, 3> Box;

	template<class T> inline void safe_delete(T*& ptr)
	{
		delete ptr;
		ptr = nullptr;
	}

	struct Elements
	{
		float32_t ecc = 0.f;
		float32_t   a = 0.f;
		float32_t  in = 0, f;
		float32_t  om = 0.f;
		float32_t   w = 0.f;
	};

	Elements const elements[] =
	{
		{ 2.056245714593660e-01f, 3.870977359489419e-01f, 7.010973469815249e+00f, 4.845668075886037e+01f, 2.884039888952456e+01f, },
		{ 6.806965099797136e-03f, 7.233254654407552e-01f, 3.395588902703575e+00f, 7.695887256790466e+01f, 5.436216459969329e+01f, },
		{ 2.328091526688903e-02f, 1.012010420088377e+00f, 8.324680150329519e-03f, 3.313925658836793e+02f, 1.410705296729917e+02f, },
		{ 9.320070091096219e-02f, 1.523645757093395e+00f, 1.857831929638693e+00f, 4.985476704371550e+01f, 2.857837022752523e+02f, },
		{ 4.896352389326375e-02f, 5.201356230578341e+00f, 1.306097293250404e+00f, 1.002776161037401e+02f, 2.736725296584393e+02f, },
		{ 5.145515520501510e-02f, 9.577378487187870e+00f, 2.482476609146576e+00f, 1.139535214399594e+02f, 3.402542855632196e+02f, },
		{ 4.893436515749554e-02f, 1.931280009054800e+01f, 7.761534668809078e-01f, 7.401222968515378e+01f, 1.031712356812917e+02f, },
		{ 5.396631185487585e-03f, 2.995505220270377e+01f, 1.766562768632231e+00f, 1.319008954718431e+02f, 2.562152041746992e+02f, },
		{ 2.478618527514649e-01f, 3.950092123894740e+01f, 1.716594041052891e+01f, 1.102440726385179e+02f, 1.151532923291780e+02f, },
		{ 4.085670288166271e-04f, 4.639091756322507e-05f, 2.993383590139660e+01f, 2.447983757684773e+02f, 2.446811656015392e+02f, },
	};

	inline LUCID_GAL::Vector2 computeConicPoint(float32_t hu, float32_t ecc, float32_t theta)
	{
		float32_t c = LUCID_MATH::cos(theta);
		float32_t s = LUCID_MATH::sin(theta);
		return LUCID_GAL::Vector2(c, s) * hu / (1.f + ecc * c);
	}

	inline LUCID_GAL::Vector3 computeConicPosition(Elements const &elmn, float32_t theta)
	{
		float32_t hu = elmn.a * (1.f - elmn.ecc * elmn.ecc);

		LUCID_GAL::Matrix3x3 R0 = LUCID_MATH::rotateAboutZ(LUCID_CORE_NUMBERS::pi<float32_t> * elmn.om / 180.f);
		LUCID_GAL::Matrix3x3 R1 = LUCID_MATH::rotateAboutX(LUCID_CORE_NUMBERS::pi<float32_t> * elmn.in / 180.f);
		LUCID_GAL::Matrix3x3 R2 = LUCID_MATH::rotateAboutZ(LUCID_CORE_NUMBERS::pi<float32_t> * elmn. w / 180.f);

		return R0 * R1 * R2 * LUCID_GAL::Vector3(computeConicPoint(hu, elmn.ecc, theta), 0.f);
	}

	LUCID_GAL::Vector3 const orbitPositions[] =
	{
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		{ 0, 0, 0, },
		computeConicPosition(elements[2], 0.1f), // this orbit is centered on earth
	};

}

void Session::initialize()
{
	shutdown();

	LUCID_GAL::Quaternion q = LUCID_MATH::rotateUsingAxis(LUCID_GAL::Vector3(1, 0, 0), 0.7f);
	LUCID_GAL::Vector3 r = LUCID_MATH::matrixFromQuaternion(q) * LUCID_GAL::Vector3(0, 1, 0);

	_viewPosition = orbitPositions[9] + computeConicPosition(elements[9], 0.f);
	_viewFocus = orbitPositions[9] + computeConicPosition(elements[9], 0.01f);

	_context = LUCID_GIGL::Context("content/render.context");

	_hemisphere = LUCID_GIGL::Mesh::create("content/hemisphere.mesh");
	_hemisphereInstances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, INSTANCE_MAXIMUM, sizeof(Instance));

	_orbit = LUCID_GIGL::Mesh::create("content/orbit.mesh");
	_orbitInstances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, INSTANCE_MAXIMUM, sizeof(Instance));

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

	_context["lightPosition"] = LUCID_GAL::Vector3(100, -100, 100);

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

#if false

	Instance* instances = (Instance*)(_hemisphereInstances->lock());
	float32_t distance = znear;
	{
		Instance& instance = instances[0];

		float32_t r1 = distance * 0.758112f / (1.f + 0.758112f);

		instance.id = 0;
		instance.position = LUCID_GAL::Vector3(r1 + distance, 0, 0) - _viewPosition;
		instance.rotation = LUCID_GAL::Quaternion();
		instance.scale = r1;
		instance.color = LUCID_GAL::Color(0, 1, 0, 0.33f);
		instance.parameters = LUCID_GAL::Vector4();

		distance = distance + r1;
	}
	_hemisphereInstances->unlock();

#endif

#if true
	Instance *instances = _orbitInstances->lock_as<Instance>();
	for (size_t i = 0; i < 10; ++i)
	{
		Instance& instance = instances[i];

		LUCID_GAL::Vector3 r = orbitPositions[i] - _viewPosition;
		float32_t d0 = LUCID_MATH::len(r);
		float32_t d1 = 250.f;

		float32_t   a = d1 * elements[i].a / d0;
		float32_t ecc = elements[i].ecc;
		float32_t  hu = a * (1.f - ecc * ecc);

		LUCID_GAL::Matrix3x3 R0 = LUCID_MATH::rotateAboutZ(LUCID_CORE_NUMBERS::pi<float32_t> *elements[i].om / 180.f);
		LUCID_GAL::Matrix3x3 R1 = LUCID_MATH::rotateAboutX(LUCID_CORE_NUMBERS::pi<float32_t> *elements[i].in / 180.f);
		LUCID_GAL::Matrix3x3 R2 = LUCID_MATH::rotateAboutZ(LUCID_CORE_NUMBERS::pi<float32_t> *elements[i]. w / 180.f);

		instance.id = uint32_t(i);
		instance.position = d1 * r / d0;
		instance.rotation = LUCID_MATH::quaternionFromMatrix(R0 * R1 * R2);
		instance.scale = 4;
		instance.color = (9 == i) ? LUCID_GAL::Color(0, 1, 0, 1) : LUCID_GAL::Color(0, 0, 1, 1);
		instance.parameters = LUCID_GAL::Vector4(hu, ecc, 0.f, LUCID_CORE_NUMBERS::two_pi<float32_t>);
	}
	_orbitInstances->unlock();
#endif
}

void Session::render(float64_t t, float32_t interpolant)
{
	if (!_initialized)
		return;

	_context["time"] = float32_t(t);
	_context["interpolant"] = float32_t(interpolant);

#if false
	LUCID_GAL_PIPELINE.setVertexStream(1, _hemisphereInstances);
	_hemisphere->renderInstanced(_context, 1);
#endif

#if true
	LUCID_GAL_PIPELINE.setVertexStream(1, _orbitInstances);
	_orbit->renderInstanced(_context, 10);
#endif
}
