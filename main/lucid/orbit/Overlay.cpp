#include "Overlay.h"
#include "Frame.h"
#include "Properties.h"
#include "Selection.h"
#include "Utility.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Resources.h>

LUCID_ANONYMOUS_BEGIN

typedef LUCID_ORBIT::NullSort<LUCID_ORBIT::IconInstance> icon_sort_t;
typedef LUCID_ORBIT::NullSort<LUCID_ORBIT::MeshInstance> mesh_sort_t;

LUCID_ANONYMOUS_END

LUCID_ORBIT_BEGIN

Overlay::Overlay()
{
}

Overlay::~Overlay()
{
	shutdown();
}

void Overlay::initialize(size_t passMaximum, float32_t midRange)
{
	shutdown();

	_passMaximum = passMaximum;
	_midRange = midRange;

	_batched.initialize();

	_orbitMesh = LUCID_GIGL_RESOURCES.get<LUCID_GIGL::Mesh>("content/orbit.mesh");
	_batched.createBatch<MeshInstance, mesh_sort_t>(_orbitMesh, _passMaximum, mesh_sort_t());

	_iconDefault = LUCID_GIGL_RESOURCES.get<LUCID_GIGL::Mesh>("content/iconDefault.mesh");
	_batched.createBatch<IconInstance, icon_sort_t>(_iconDefault, _passMaximum, icon_sort_t());

	_iconSatellite = LUCID_GIGL_RESOURCES.get<LUCID_GIGL::Mesh>("content/iconSatellite.mesh");
	_batched.createBatch<IconInstance, icon_sort_t>(_iconSatellite, _passMaximum, icon_sort_t());

}

void Overlay::shutdown()
{
	_batched.shutdown();

	_iconDefault.reset();
	_iconSatellite.reset();
	_orbitMesh.reset();

	_midRange = 0.f;
	_passMaximum = 0;
}

void Overlay::process(Frame *frame, CameraFrame *cameraFrame, scalar_t interpolant)
{
	_interpolant = interpolant;
	_cameraPosition = LUCID_MATH::lerp(_interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);

	process(frame);
}

void Overlay::render(LUCID_GIGL::Context const &context)
{
	_batched.render(context);
	_batched.clear();
}

void Overlay::evaluate(DynamicPoint *point)
{
	/// TBD: implement...
}

void Overlay::evaluate(OrbitalBody *body)
{
	batchOrbit(body);
	batchIcon(body);
}

void Overlay::evaluate(DynamicBody *body)
{
	/// TBD: implement...
}

void Overlay::evaluate(CameraFrame *camera)
{
	/// TBD: implement...
}

void Overlay::process(Frame *frame)
{
	if (nullptr == frame)
		return;

	frame->apply(this);

	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		process(child);
}

void Overlay::batchOrbit(OrbitalBody *body)
{
	RenderProperties const &renderProperties = body->renderProperties;
	if (!renderProperties.showOrbit)
		return;

	PhysicalProperties const &physicalProperties = body->physicalProperties;
	Elements const *elements = body->elements;

	Frame const *centerFrame = body->centerFrame;

	vector3_t centerPosition = LUCID_MATH::lerp(_interpolant, centerFrame->absolutePosition[0], centerFrame->absolutePosition[1]) - _cameraPosition;
	scalar_t centerDistance = LUCID_MATH::len(centerPosition);

	quaternion_t q0 = LUCID_MATH::quaternionFromMatrix(rotationFromElements(elements[0]));
	quaternion_t q1 = LUCID_MATH::quaternionFromMatrix(rotationFromElements(elements[1]));
	quaternion_t  q = LUCID_MATH::slerp(_interpolant, q0, q1);

	vector3_t relPosition = LUCID_MATH::lerp(_interpolant, body->relativePosition[0], body->relativePosition[1]);
	relPosition = relPosition * LUCID_MATH::matrixFromQuaternion(q);

	// compute theta, the angular position of the body
	float32_t theta = cast(LUCID_MATH::atan2(relPosition.y, relPosition.x));
	theta = (theta > 0.f) ? theta : LUCID_CORE_NUMBERS::two_pi<float32_t> + theta;

	float32_t  a = _midRange * cast(LUCID_MATH::lerp(_interpolant, elements[0].A, elements[1].A) / centerDistance);
	float32_t  e = cast(LUCID_MATH::lerp(_interpolant, elements[0].EC, elements[1].EC));
	float32_t hu = a * (1.f - e * e);

	MeshInstance orbitInstance;
	orbitInstance.id = uint32_t((Selection::TYPE_ORBIT << Selection::SELECT_SHIFT) | body->id);
	orbitInstance.position = _midRange * cast(centerPosition / centerDistance);
	orbitInstance.scale = 4;
	orbitInstance.rotation = cast(q);
	orbitInstance.channel0 = renderProperties.orbitHighlight ? LUCID_GAL::Vector4(1.f, 1.f, 1.f, 1.f) : LUCID_GAL::Vector4(0, 0, 1, 1);
	orbitInstance.channel1 = LUCID_GAL::Vector4(0, 0, 0, cast(centerDistance / _midRange));
	orbitInstance.channel2 = LUCID_GAL::Vector4(hu, e, theta + 0.01f, theta + (LUCID_CORE_NUMBERS::two_pi<float32_t> - 0.01f));
	_batched.addInstance(_orbitMesh, orbitInstance);
}

void Overlay::batchIcon(OrbitalBody *body)
{
	if (Frame::CULL_STATE_IMPERCEPTIBLE != body->cullState)
		return;

	RenderProperties const &renderProperties = body->renderProperties;

	vector3_t bodyPosition = LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition;
	scalar_t bodyDistance = LUCID_MATH::len(bodyPosition);

	IconInstance iconInstance;
	iconInstance.id = uint32_t((Selection::TYPE_CALLOUT << Selection::SELECT_SHIFT) | body->id);
	iconInstance.position = LUCID_GAL::Vector4(_midRange * cast(bodyPosition / bodyDistance), cast(bodyDistance / _midRange));
	iconInstance.scale = LUCID_GAL::Vector2(24, 24);
	iconInstance.texcoord = LUCID_GAL::Vector4(0, 0, 1, 1);
	iconInstance.color = LUCID_GAL::Color(1, 1, 1, 1);
	_batched.addInstance(renderProperties.icon, iconInstance);
}

LUCID_ORBIT_END
