#pragma once

#include <string>
#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Profiler.h>
#include <lucid/gal/Defines.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Font.h>
#include <lucid/gigl/Batched.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Algorithm.h>
#include <lucid/orbit/Culler.h>

LUCID_GAL_BEGIN

class Parameter;
class VertexBuffer;
class RenderTarget2D;
class TargetReader2D;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Mesh;

LUCID_GIGL_END

LUCID_ORBIT_BEGIN

///
///
///
class Renderer : public Algorithm
{
public:
	///	SELECT
	/// 
	///	Codes used to render into the render target.
	/// Used for "pixel perfect" selection.
	enum SELECT
	{
		SELECT_STAR		= 0x01,
		SELECT_FRAME	= 0x02,
		SELECT_ORBIT	= 0x03,
		SELECT_CAMERA	= 0x04,
		SELECT_CALLOUT  = 0x05,
		SELECT_OTHER	= 0x0f,
	};
	enum { SELECT_SHIFT = 28 };
	enum { SELECT_MASK  = 0x0fffffff};

	Renderer();

	virtual ~Renderer();

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

	void initialize(std::string const &path);

	void shutdown();

	void render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant, bool useFXAA = false);

	uint32_t hit(int32_t x, int32_t y) const;

private:
	enum {       BATCH_MAXIMUM = 1024 };
	enum { TEXT_LENGTH_MAXIMUM = 4096 };

	struct StarParameters
	{
		LUCID_GAL::Parameter const *sphereRadius = nullptr;
		LUCID_GAL::Parameter const *spriteScale = nullptr;
	};

	struct CopyParameters
	{
		LUCID_GAL::Parameter const *theSource = nullptr;
	};

	struct BlurParameters
	{
		LUCID_GAL::Parameter const *texelOffset = nullptr;
		LUCID_GAL::Parameter const *theSource = nullptr;
	};

	struct PostParameters
	{
		LUCID_GAL::Parameter const *colorTarget = nullptr;
		LUCID_GAL::Parameter const *glowTarget = nullptr;
	};

	struct StarInstance
	{
		uint32_t id;
		LUCID_GAL::Vector4 parameters;
		LUCID_GAL::Color color;
	};

	struct MeshInstance
	{
		uint32_t                      id;
		LUCID_GAL::Vector3      position;
		float32_t                  scale;
		LUCID_GAL::Quaternion   rotation;
		LUCID_GAL::Color           color;
		LUCID_GAL::Vector4    parameters;
	};

	struct IconInstance
	{
		uint32_t                 id;
		LUCID_GAL::Vector3 position;
		LUCID_GAL::Vector4 texcoord;
		LUCID_GAL::Vector2    scale;
		LUCID_GAL::Color      color;
	};

	int32_t _width = 0;
	int32_t _height = 0;

	scalar_t _interpolant = 0.0;

	vector3_t _cameraPosition;
	vector3_t _focusPosition;

	Culler _culler;

	LUCID_GIGL::Context _renderContext;

	size_t _starCount = 0;
	StarParameters _starParameters;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _starInstances;
	std::unique_ptr<LUCID_GIGL::Mesh> _starMesh;

	LUCID_GIGL::Batched _sceneBatch;

	std::shared_ptr<LUCID_GIGL::Mesh> _orbitMesh;
	LUCID_GIGL::Batched _overlayBatch;

	std::shared_ptr<LUCID_GIGL::Font> _font;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _text;
	int32_t _textCount = 0;

	std::unique_ptr<LUCID_GAL::RenderTarget2D> _colorTarget;
	std::unique_ptr<LUCID_GAL::RenderTarget2D> _glowTarget;
	std::unique_ptr<LUCID_GAL::RenderTarget2D> _blurTarget[2];

	std::unique_ptr<LUCID_GAL::RenderTarget2D> _selectTarget;
	std::unique_ptr<LUCID_GAL::TargetReader2D> _selectReader;

	std::unique_ptr<LUCID_GIGL::Mesh> _clear;
		
	CopyParameters _copyParameters;
	std::unique_ptr<LUCID_GIGL::Mesh> _copy;
		
	BlurParameters _blurParameters;
	std::unique_ptr<LUCID_GIGL::Mesh> _blur;

	PostParameters _postParameters;
	std::unique_ptr<LUCID_GIGL::Mesh> _post;

	PostParameters _fxaaParameters;
	std::unique_ptr<LUCID_GIGL::Mesh> _fxaa;

	LUCID_GAL::Scalar adaptiveScale(scalar_t const &value);

	LUCID_GAL::Vector3 adaptiveScale(vector3_t const &value);

	void batch(Frame *frame);

	void render(bool useFXAA);

	void renderScene();

	void renderOverlay();

	void renderStarfield();

	void copy(LUCID_GAL::RenderTarget2D *dst, LUCID_GAL::RenderTarget2D *src);

	void blur();

	void scenePost();

	void fxaaScenePost();

	void resize();

	LUCID_PREVENT_COPY(Renderer);
	LUCID_PREVENT_ASSIGNMENT(Renderer);
};

inline LUCID_GAL::Scalar Renderer::adaptiveScale(scalar_t const &value)
{
	scalar_t result = _culler.sceneScalingFactor * value;
	return LUCID_GAL::Scalar(float32_t(result.value));
}

inline LUCID_GAL::Vector3 Renderer::adaptiveScale(vector3_t const &value)
{
	return LUCID_GAL::Vector3(adaptiveScale(value.x), adaptiveScale(value.y), adaptiveScale(value.z));
}

LUCID_ORBIT_END