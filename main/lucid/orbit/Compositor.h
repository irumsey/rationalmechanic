#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <lucid/gal/Defines.h>
#include <lucid/gigl/Types.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Algorithm.h>

LUCID_GAL_BEGIN

class VertexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Context;
class Model;

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

	void initialize(size_t passMaximum, float32_t midRange);

	void shutdown();

	void process(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t interpolant);

	void render(LUCID_GIGL::Context const &context);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	///	Pass
	///
	///	In the near future, this might become generalized to
	/// allow for any kind of render pass (not just a model).
	struct Pass
	{
		std::shared_ptr<LUCID_GIGL::Model> model;

		uint32_t                    id = 0;
		vector3_t             position;
		scalar_t              distance = 0;
		quaternion_t          rotation;
		scalar_t           scaleFactor = 0;
		LUCID_GAL::Color       diffuse;
		LUCID_GAL::Color       ambient;
		LUCID_GAL::Vector4  parameters;
	};

#	pragma pack(push)
#	pragma pack(1)

	struct ModelInstance
	{
		uint32_t                      id = 0;
		LUCID_GAL::Vector3      position;		//	position and scale are "packed" into
		float32_t                  scale = 0;	//	a hlsl float4 type
		LUCID_GAL::Quaternion   rotation;
		LUCID_GAL::Color         diffuse;
		LUCID_GAL::Color	     ambient;
		LUCID_GAL::Vector4    parameters;
	};

#	pragma pack(pop)

	size_t       _passMaximum = 0;
	float32_t       _midRange = 0;

	scalar_t     _interpolant = 0;
	vector3_t _cameraPosition;

	std::vector<Pass> _passes;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _instanceStream;

	void composite(Frame *frame);

	void copyInstances();

	LUCID_PREVENT_COPY(Compositor);
	LUCID_PREVENT_ASSIGNMENT(Compositor);
};

LUCID_ORBIT_END
