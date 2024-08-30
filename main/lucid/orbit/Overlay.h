#pragma once

#include <memory>
#include <lucid/core/Noncopyable.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Batched.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Algorithm.h>

LUCID_GIGL_BEGIN

class Context;

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

	void process(Frame *frame, CameraFrame *cameraFrame, scalar_t interpolant);

	void render(LUCID_GIGL::Context const &context);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	size_t _passMaximum = 0;
	float32_t _midRange = 0.f;

	scalar_t _interpolant = 0.0;
	vector3_t _cameraPosition;

	std::shared_ptr<LUCID_GIGL::Mesh> _orbitMesh;
	std::shared_ptr<LUCID_GIGL::Mesh> _iconDefault;
	std::shared_ptr<LUCID_GIGL::Mesh> _iconSatellite;

	LUCID_GIGL::Batched _batched;

	void process(Frame *frame);

	void batchOrbit(OrbitalBody *body);

	void batchIcon(OrbitalBody *body);

	LUCID_PREVENT_ASSIGNMENT(Overlay);
	LUCID_PREVENT_COPY(Overlay);
};

LUCID_ORBIT_END
