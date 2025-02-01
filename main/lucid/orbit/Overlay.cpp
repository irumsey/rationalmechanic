#include "Overlay.h"
#include "Frame.h"
#include "Properties.h"
#include "Selection.h"
#include "Utility.h"
#include <lucid/gigl/Font.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Pipeline.h>

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

	_stdIcons = LUCID_GIGL_RESOURCES.get<LUCID_GIGL::Mesh>("content/stdIcons.mesh");
	_batched.createBatch<IconInstance, icon_sort_t>(_stdIcons, _passMaximum, icon_sort_t());

	_font = LUCID_GIGL::Resources::get<LUCID_GIGL::Font>("content/OCRa.font");
	_text.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, TEXT_LENGTH_MAXIMUM, sizeof(LUCID_GIGL::Font::Character)));
}

void Overlay::shutdown()
{
	_text.reset();
	_font.reset();

	_batched.shutdown();

	_stdIcons.reset();

	_orbitMesh.reset();

	_midRange = 0.f;
	_passMaximum = 0;
}

void Overlay::print(LUCID_GAL::Vector2 const &position, float32_t size, std::string const &text, LUCID_GAL::Color const &color)
{
	int32_t count = int32_t(text.size());

	_font->typeset(_text->lock_as<LUCID_GIGL::Font::Character>(_textCount, count), LUCID_GIGL::Font::ALIGN_LEFT , position, size, text, color);
	_text->unlock();

	_textCount = _textCount + count;
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

	if (0 == _textCount)
		return;

	LUCID_GAL_PIPELINE.setVertexStream(1, _text.get());
	_font->renderInstanced(context, _textCount);

	_textCount = 0;
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
	OrbitalElements const *orbitalElements = body->orbitalElements;

	Frame const *centerFrame = body->centerFrame;

	vector3_t centerPosition = LUCID_MATH::lerp(_interpolant, centerFrame->absolutePosition[0], centerFrame->absolutePosition[1]) - _cameraPosition;
	scalar_t centerDistance = LUCID_MATH::len(centerPosition);

	quaternion_t q0 = LUCID_MATH::quaternionFromMatrix(rotationFromElements(orbitalElements[0]));
	quaternion_t q1 = LUCID_MATH::quaternionFromMatrix(rotationFromElements(orbitalElements[1]));
	quaternion_t  q = LUCID_MATH::slerp(_interpolant, q0, q1);

	vector3_t relPosition = LUCID_MATH::lerp(_interpolant, body->relativePosition[0], body->relativePosition[1]);
	relPosition = relPosition * LUCID_MATH::matrixFromQuaternion(q);

	// compute theta, the angular position of the body
	float32_t theta = cast(LUCID_MATH::atan2(relPosition.y, relPosition.x));
	theta = (theta > 0.f) ? theta : LUCID_CORE_NUMBERS::two_pi<float32_t> + theta;

	scalar_t a = LUCID_MATH::lerp(_interpolant, orbitalElements[0]. A, orbitalElements[1]. A);
	scalar_t e = LUCID_MATH::lerp(_interpolant, orbitalElements[0].EC, orbitalElements[1].EC);

	scalar_t scaleFactor = a / centerDistance;
	a = _midRange * scaleFactor;

	scalar_t hu = a * (1.0 - e * e);

	MeshInstance orbitInstance;
	orbitInstance.id = Selection(Selection::TYPE_ORBIT, body->id).token;
	orbitInstance.position = _midRange * cast(centerPosition / centerDistance);
	orbitInstance.scale = 3;
	orbitInstance.rotation = cast(q);
	orbitInstance.lightPosition = LUCID_GAL::Vector4(0, 0, 0, 0);
	orbitInstance.compositing = cast(vector4_t(scaleFactor, centerDistance / _midRange, 0, 0));
	orbitInstance.channel0 = renderProperties.orbitHighlight ? LUCID_GAL::Vector4(1.f, 1.f, 1.f, 1.f) : LUCID_GAL::Vector4(0, 0, 1, 1);
	orbitInstance.channel1 = LUCID_GAL::Vector4(0, 0, 0, 0);
	orbitInstance.channel2 = cast(vector4_t(hu, e, theta + 0.01f, theta + (LUCID_CORE_NUMBERS::two_pi<float32_t> - 0.01f)));
	_batched.addInstance(_orbitMesh, orbitInstance);
}

void Overlay::batchIcon(OrbitalBody *body)
{
	if (Frame::CULL_STATE_IMPERCEPTIBLE != body->cullState)
		return;

	RenderProperties const &renderProperties = body->renderProperties;

	if (!renderProperties.showIcon)
		return;

	vector3_t bodyPosition = LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition;
	scalar_t bodyDistance = LUCID_MATH::len(bodyPosition);

	IconInstance iconInstance;
	iconInstance.id = Selection(Selection::TYPE_ICON, body->id).token;
	iconInstance.position = LUCID_GAL::Vector4(_midRange * cast(bodyPosition / bodyDistance), cast(bodyDistance / _midRange));
	iconInstance.scale = LUCID_GAL::Vector2(24, 24);
	iconInstance.texcoord = renderProperties.iconParameters;
	iconInstance.color = LUCID_GAL::Color(1, 1, 1, 1);
	_batched.addInstance(renderProperties.iconMesh, iconInstance);
}

LUCID_ORBIT_END
