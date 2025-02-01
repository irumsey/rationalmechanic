#pragma once

#include <memory>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Defines.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Batched.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Algorithm.h>

LUCID_GAL_BEGIN

class VertexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Context;
class Font;

LUCID_GIGL_END

LUCID_ORBIT_BEGIN

class Frame;
class DynamicPoint;
class OrbitalBody;
class DynamicBody;
class CameraFrame;

/// Overlay
///
/// 
class Overlay : public Algorithm
{
public:
	Overlay();

	virtual ~Overlay();

	void initialize(size_t passMaximum, float32_t midRange);

	void shutdown();

	void print(LUCID_GAL::Vector2 const &position, float32_t size, std::string const &text, LUCID_GAL::Color const &color);

	void process(Frame *frame, CameraFrame *cameraFrame, scalar_t interpolant);

	void render(LUCID_GIGL::Context const &context);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	enum { TEXT_LENGTH_MAXIMUM = 4096 };

	size_t _passMaximum = 0;
	float32_t _midRange = 0.f;

	scalar_t _interpolant = 0.0;
	vector3_t _cameraPosition;

	std::shared_ptr<LUCID_GIGL::Mesh> _orbitMesh;
	std::shared_ptr<LUCID_GIGL::Mesh> _stdIcons;

	LUCID_GIGL::Batched _batched;

	std::shared_ptr<LUCID_GIGL::Font> _font;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _text;
	int32_t _textCount = 0;

	void process(Frame *frame);

	void batchOrbit(OrbitalBody *body);

	void batchIcon(OrbitalBody *body);

	LUCID_PREVENT_ASSIGNMENT(Overlay);
	LUCID_PREVENT_COPY(Overlay);
};

LUCID_ORBIT_END
