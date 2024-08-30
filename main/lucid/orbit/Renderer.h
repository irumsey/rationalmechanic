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
#include <lucid/orbit/Utility.h>
#include <lucid/orbit/Selection.h>
#include <lucid/orbit/Culler.h>
#include <lucid/orbit/Compositor.h>
#include <lucid/orbit/Overlay.h>

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

///	Renderer
///
///
class Renderer
{
public:
	Renderer();

	virtual ~Renderer();

	void initialize(std::string const &path);

	void shutdown();

	void render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant, bool useFXAA = false);

	Selection hit(int32_t x, int32_t y) const;

private:
	enum {       BATCH_MAXIMUM = 1024 };
	enum { TEXT_LENGTH_MAXIMUM = 4096 };

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

	///	test {
	/// magic numbers
	float32_t _znear = 10.f;
	float32_t _zfar = 1000.f;
	float32_t _zmid = 0.5f * (_znear + _zfar);
	/// } test

	int32_t _width = 0;
	int32_t _height = 0;
	float32_t _aspect = 1.f;
	LUCID_GAL::Vector2 _screenSize;
	LUCID_GAL::Vector2 _texelSize;
	
	scalar_t _interpolant = 0.0;

	vector3_t _cameraPosition;
	vector3_t _focusPosition;

	Culler _culler;
	Compositor _compositor;
	Overlay _overlay;

	LUCID_GIGL::Context _renderContext;

	std::shared_ptr<LUCID_GIGL::Font> _font;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _text;
	int32_t _textCount = 0;

	std::unique_ptr<LUCID_GAL::RenderTarget2D> _colorTarget;
	std::unique_ptr<LUCID_GAL::RenderTarget2D> _depthTarget;
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

	void preRender();

	void render();

	void postRender(bool useFXAA);

	void copy(LUCID_GAL::RenderTarget2D *dst, LUCID_GAL::RenderTarget2D *src);

	void blur();

	void resize();

	LUCID_PREVENT_COPY(Renderer);
	LUCID_PREVENT_ASSIGNMENT(Renderer);
};

LUCID_ORBIT_END