#include "Renderer.h"
#include "Ephemeris.h"
#include "Frame.h"
#include "Utility.h"
#include "Constants.h"
#include <lucid/gigl/Model.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Camera2D.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/TargetReader2D.h>
#include <lucid/gal/RenderTarget2D.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Program.h>
#include <lucid/gal/System.h>
#include <lucid/math/Math.h>
#include <algorithm>

#include <lucid/gigl/Font.h>

// give these macros a try, see if i like them...
#define GET_MATERIAL_PARAMETER(mesh, name) mesh->material()->program()->lookup( #name )
#define SET_MATERIAL_PARAMETER(mesh, param, value) mesh->material()->program()->set(param, value)

LUCID_ANONYMOUS_BEGIN

inline LUCID_ORBIT::Ephemeris &theEphemeris()
{
	return LUCID_ORBIT::Ephemeris::instance();
}

LUCID_ANONYMOUS_END

LUCID_ORBIT_BEGIN

///
///
///

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	shutdown();
}

void Renderer::initialize(std::string const &path)
{
	shutdown();

	_renderContext = LUCID_GIGL::Context(path);

	_width = LUCID_GAL_SYSTEM.width();
	_height = LUCID_GAL_SYSTEM.height();

	_screenSize = LUCID_GAL::Vector2(float32_t(_width), float32_t(_height));
	_texelSize = LUCID_GAL::Vector2(1.f / _screenSize.x, 1.f / _screenSize.y);

	_compositor.initialize(BATCH_MAXIMUM, _zmid);
	_overlay.initialize(BATCH_MAXIMUM, _zmid);

	_selectTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
	_selectReader.reset(LUCID_GAL::TargetReader2D::create(_selectTarget.get(), _width, _height));

	_colorTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_depthTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_FLOAT_R32, _width, _height));
	_glowTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[0].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[1].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));

	_clear.reset(LUCID_GIGL::Mesh::create("content/clear.mesh"));
	_copy .reset(LUCID_GIGL::Mesh::create("content/copy.mesh"));
	_blur .reset(LUCID_GIGL::Mesh::create("content/blur.mesh"));
	_post .reset(LUCID_GIGL::Mesh::create("content/post.mesh"));
	_fxaa .reset(LUCID_GIGL::Mesh::create("content/fxaa.mesh"));

	_copyParameters.   theSource = GET_MATERIAL_PARAMETER(    _copy,    theSource);
	_blurParameters. texelOffset = GET_MATERIAL_PARAMETER(    _blur,  texelOffset);
	_blurParameters.   theSource = GET_MATERIAL_PARAMETER(    _blur,    theSource);
	_postParameters. colorTarget = GET_MATERIAL_PARAMETER(    _post,  colorTarget);
	_postParameters.  glowTarget = GET_MATERIAL_PARAMETER(    _post,   glowTarget);
	_fxaaParameters. colorTarget = GET_MATERIAL_PARAMETER(    _fxaa,  colorTarget);
	_fxaaParameters.  glowTarget = GET_MATERIAL_PARAMETER(    _fxaa,   glowTarget);

	_renderContext.add("depthTarget", _depthTarget);
}

void Renderer::shutdown()
{
	_overlay.shutdown();
	_compositor.shutdown();

	_selectReader.reset();
	_selectTarget.reset();

	_colorTarget.reset();
	_depthTarget.reset();
	_glowTarget.reset();
	_blurTarget[0].reset(); _blurTarget[1].reset();

	_clear.reset();
	_copy.reset();
	_blur.reset();
	_post.reset();
	_fxaa.reset();

	_renderContext = LUCID_GIGL::Context();
}

void Renderer::print(LUCID_GAL::Vector2 const &position, float32_t size, std::string const &text, LUCID_GAL::Color const &color)
{
	_overlay.print(position, size, text, color);
}

void Renderer::render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant, bool useFXAA)
{
	LUCID_PROFILE_SCOPE("Renderer::render(...)");
	LUCID_VALIDATE(nullptr != cameraFrame->focus, "camera does not have a focus specified");

	resize();

	float32_t fov = cast(cameraFrame->fov);
	Frame *focusFrame = cameraFrame->focus;

	vector3_t cameraPosition = LUCID_MATH::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);
	vector3_t focusPosition = LUCID_MATH::lerp(interpolant, focusFrame->absolutePosition[0], focusFrame->absolutePosition[1]) - cameraPosition;

	_culler.cull(rootFrame, cameraFrame, interpolant);
	_compositor.process(rootFrame, cameraFrame, interpolant);
	_overlay.process(rootFrame, cameraFrame, interpolant);

	_renderContext["screenSize"] = _screenSize;
	_renderContext["texelSize"] = _texelSize;

	_renderContext["time"] = cast(time);
	_renderContext["interpolant"] = cast(interpolant);
		
	LUCID_GAL::Matrix4x4 viewMatrix = LUCID_MATH::look(LUCID_GAL::Vector3(0, 0, 0), cast(focusPosition), LUCID_GAL::Vector3(0, 0, 1));
	LUCID_GAL::Matrix4x4 projMatrix = LUCID_MATH::perspective(fov, _aspect, _znear, _zfar);
	LUCID_GAL::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;

	_renderContext["viewPosition"] = LUCID_GAL::Vector3(0, 0, 0);
	_renderContext["viewForward"] = LUCID_MATH::extractViewForward(viewMatrix);
	_renderContext["viewRight"] = LUCID_MATH::extractViewRight(viewMatrix);
	_renderContext["viewUp"] = LUCID_MATH::extractViewUp(viewMatrix);
	_renderContext["viewMatrix"] = viewMatrix;
	_renderContext["invViewMatrix"] = LUCID_MATH::inverse(viewMatrix);

	_renderContext["projMatrix"] = projMatrix;
	_renderContext["invProjMatrix"] = LUCID_MATH::inverse(projMatrix);
	_renderContext["viewProjMatrix"] = viewProjMatrix;
	_renderContext["invViewProjMatrix"] = LUCID_MATH::inverse(viewProjMatrix);

	// set it each time, just in case the target was re-created since initialization
	_renderContext["depthTarget"] = _depthTarget;

	preRender();
	render();
	postRender(useFXAA);
}

Selection Renderer::hit(int32_t x, int32_t y) const
{
	x = LUCID_MATH::clamp(x, 0,  _width - 1);
	y = LUCID_MATH::clamp(y, 0, _height - 1);

	uint32_t const *target = (uint32_t const *)(_selectReader->read());
	uint32_t code = target[y * _width + x];

	return Selection(code);
}

void Renderer::preRender()
{
	LUCID_GAL_PIPELINE.setRenderTarget(0, _colorTarget.get());
	LUCID_GAL_PIPELINE.setRenderTarget(1, _glowTarget.get());
	LUCID_GAL_PIPELINE.setRenderTarget(2, _selectTarget.get());
	LUCID_GAL_PIPELINE.setRenderTarget(3, _depthTarget.get());

	LUCID_GAL_PIPELINE.updateTargets();

	_clear->render(_renderContext);
}

void Renderer::render()
{
	_compositor.render(_renderContext);

	LUCID_GAL_PIPELINE.setRenderTarget(3, nullptr);
	LUCID_GAL_PIPELINE.updateTargets();

	_overlay.render(_renderContext);
}

void Renderer::postRender(bool useFXAA)
{
	LUCID_GAL_PIPELINE.restoreBackBuffer(true, false, false);
	LUCID_GAL_PIPELINE.updateTargets();

	copy(_blurTarget[0].get(), _glowTarget.get());
	blur();
	blur();
	blur();
	copy(_glowTarget.get(), _blurTarget[0].get());

	LUCID_GAL_PIPELINE.restoreBackBuffer(true, false, false);
	LUCID_GAL_PIPELINE.updateTargets();

	if (useFXAA)
	{
		SET_MATERIAL_PARAMETER(_fxaa, _fxaaParameters.colorTarget, _colorTarget.get());
		SET_MATERIAL_PARAMETER(_fxaa, _fxaaParameters.glowTarget, _glowTarget.get());
		_fxaa->render(_renderContext);
	}
	else
	{
		SET_MATERIAL_PARAMETER(_post, _postParameters.colorTarget, _colorTarget.get());
		SET_MATERIAL_PARAMETER(_post, _postParameters.glowTarget, _glowTarget.get());
		_post->render(_renderContext);
	}
}

void Renderer::copy(LUCID_GAL::RenderTarget2D *dst, LUCID_GAL::RenderTarget2D *src)
{
	LUCID_GAL_PIPELINE.setRenderTarget(0, dst);
	LUCID_GAL_PIPELINE.setRenderTarget(1, nullptr);
	LUCID_GAL_PIPELINE.setRenderTarget(2, nullptr);
	LUCID_GAL_PIPELINE.setRenderTarget(3, nullptr);
	LUCID_GAL_PIPELINE.updateTargets();
	
	SET_MATERIAL_PARAMETER(_copy, _copyParameters.theSource, src);
	_copy->render(_renderContext);
}

void Renderer::blur()
{
	LUCID_GAL::Vector2 horizontal = LUCID_GAL::Vector2(_texelSize.x,          0.f);
	LUCID_GAL::Vector2   vertical = LUCID_GAL::Vector2(         0.f, _texelSize.y);

	LUCID_GAL_PIPELINE.setRenderTarget(0, _blurTarget[1].get());
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(_blur, _blurParameters.theSource, _blurTarget[0].get());
	SET_MATERIAL_PARAMETER(_blur, _blurParameters.texelOffset, horizontal);
	_blur->render(_renderContext);

	LUCID_GAL_PIPELINE.setRenderTarget(0, _blurTarget[0].get());
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(_blur, _blurParameters.theSource, _blurTarget[1].get());
	SET_MATERIAL_PARAMETER(_blur, _blurParameters.texelOffset, vertical);
	_blur->render(_renderContext);
}

void Renderer::resize()
{
	if ((_width == LUCID_GAL_SYSTEM.width()) && (_height == LUCID_GAL_SYSTEM.height()))
		return;

	_width = LUCID_GAL_SYSTEM.width();
	_height = LUCID_GAL_SYSTEM.height();
	_aspect = LUCID_GAL_SYSTEM.aspect();

	_screenSize = LUCID_GAL::Vector2(float32_t(_width), float32_t(_height));
	_texelSize = LUCID_GAL::Vector2(1.f / _screenSize.x, 1.f / _screenSize.y);

	_selectTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
	_selectReader.reset(LUCID_GAL::TargetReader2D::create(_selectTarget.get(), _width, _height));

	_colorTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_depthTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_FLOAT_R32, _width, _height));
	_glowTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[0].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[1].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
}

LUCID_ORBIT_END