#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <lucid/gal/Defines.h>
#include <lucid/gigl/Types.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Algorithm.h>

LUCID_GAL_BEGIN

class VertexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Context;
class Model;
class Mesh;

LUCID_GIGL_END

LUCID_ORBIT_BEGIN

class Frame;
class DynamicPoint;
class OrbitalBody;
class DynamicBody;
class CameraFrame;

/// 
/// 
/// 
class Compositor : public Algorithm
{
public:
	Compositor();

	virtual ~Compositor();

	void initialize(size_t passMaximum, scalar_t midRange);

	void shutdown();

	void process(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t interpolant);

	void render(LUCID_GIGL::Context const &context);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	size_t _starCount = 0;
	std::unique_ptr<LUCID_GIGL::Mesh> _starMesh;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _starInstances;

	///	Pass
	///
	///	In the near future, this might become generalized to
	/// allow for any kind of compositing pass (not just for models).
	struct Pass
	{
		std::shared_ptr<LUCID_GIGL::Model> model;

		uint32_t                      id = 0;
		vector3_t               position;
		scalar_t                distance = 0;
		quaternion_t            rotation;
		vector4_t          lightPosition;
		vector4_t            compositing;
		LUCID_GAL::Vector4      channel0;
		LUCID_GAL::Vector4      channel1;
		LUCID_GAL::Vector4      channel2;
	};

	size_t       _passMaximum = 0;
	scalar_t        _midRange = 0;

	scalar_t     _interpolant = 0;
	vector3_t _cameraPosition;

	vector4_t  _lightPosition;

	std::vector<Pass> _passes;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _meshInstances;

	void process(Frame *frame);

	void copyInstances();

	LUCID_PREVENT_COPY(Compositor);
	LUCID_PREVENT_ASSIGNMENT(Compositor);
};

LUCID_ORBIT_END
