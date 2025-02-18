#include "State.h"
#include "Session.h"
#include <lucid/gui/Checkbox.h>
#include <lucid/gui/Button.h>
#include <lucid/gui/Label.h>
#include <lucid/gui/Panel.h>
#include <lucid/gui/Frame.h>
#include <lucid/gui/Events.h>
#include <lucid/gui/Types.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Context.h>
#include <lucid/gal/RenderTarget2D.h>
#include <lucid/gal/TargetReader2D.h>
#include <lucid/gal/Program.h>
#include <lucid/gal/Parameter.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/core/Memory.h>

// test {
#include <sstream>
#include <iomanip>

float64_t const SECONDS_PER_DAY = 24.0 * 3600.0;
float64_t const DAYS_PER_SECOND = 1.0 / SECONDS_PER_DAY;

// give these macros a try, see if i like them...
#define GET_MATERIAL_PARAMETER(mesh, name) mesh->program()->lookup( #name )
#define SET_MATERIAL_PARAMETER(mesh, param, value) mesh->program()->set( param, value )

// } test

namespace /* anonymous */ {

namespace gui = LUCID_GUI;

enum ID
{
	ID_NONE				=     0,
		
	ID_BTN_START		= 0x101,

	ID_BTN_STOP			= 0x201,
	ID_BTN_SLOWER		= 0x202,
	ID_BTN_PLAYPAUSE	= 0x203,
	ID_BTN_FASTER		= 0x204,
	ID_LBL_TIME			= 0x205,
	ID_LBL_SELECT		= 0x206,
};

// test {
// assuming the layout in a 1024x1024 texture
gui::Button::Tiles const startTiles = {
	LUCID_GAL::Vector4( 0.f,   0.f, 96.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 0.f,  96.f, 96.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 0.f, 192.f, 96.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 0.f, 288.f, 96.f, 384.f) / 1024.f,
};
gui::Checkbox::Tiles const playPauseTiles = {
	LUCID_GAL::Vector4(  0.f,   0.f,  96.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4(  0.f,  96.f,  96.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4(  0.f,   0.f,  96.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4(  0.f, 192.f,  96.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f,   0.f, 192.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f,  96.f, 192.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f,   0.f, 192.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f, 192.f, 192.f, 288.f) / 1024.f,
};
gui::Button::Tiles const stopTiles = {
	LUCID_GAL::Vector4( 192.f,   0.f, 288.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 192.f,  96.f, 288.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 192.f, 192.f, 288.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 192.f, 288.f, 288.f, 384.f) / 1024.f,
};
gui::Button::Tiles const fasterTiles = {
	LUCID_GAL::Vector4( 288.f,   0.f, 384.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 288.f,  96.f, 384.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 288.f, 192.f, 384.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 288.f, 288.f, 384.f, 384.f) / 1024.f,
};
gui::Button::Tiles const slowerTiles = {
	LUCID_GAL::Vector4( 384.f,   0.f, 480.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 384.f,  96.f, 480.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 384.f, 192.f, 480.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 384.f, 288.f, 480.f, 384.f) / 1024.f,
};
// } test

}	// anonymous

///
/// 
/// 

void State::onEvent(Session *session, gui::SizeEvent const &event) const 
{
	int32_t width = event.width();
	int32_t height = event.height();

	LUCID_CORE::reset_raw_ptr(session->_selectTarget);
	LUCID_CORE::reset_raw_ptr(session->_colorTarget);
	LUCID_CORE::reset_raw_ptr(session->_glowTarget);
	LUCID_CORE::reset_raw_ptr(session->_depthTarget);

	LUCID_CORE::reset_raw_ptr(session->_selectReader);

	session->_selectTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, width, height);
	session->_colorTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, width, height);
	session->_glowTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, width, height);
	session->_depthTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_FLOAT_R32, width, height);

	session->_selectReader = LUCID_GAL::TargetReader2D::create(session->_selectTarget, width, height);

	session->_blurTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, width, height);

	session->_guiConfiguring->onEvent(event);
	session->_guiRunning->onEvent(event);
}

void State::preRender(Session *session, float64_t time) const
{
	LUCID_GUI::Rectangle const &rectangle = session->_rectangle;
	LUCID_GIGL::Context &context = session->_renderContext;

	float32_t  width = float32_t(rectangle.max.x - rectangle.min.x);
	float32_t height = float32_t(rectangle.max.y - rectangle.min.y);

	context[      "time"] = float32_t(time);
	context["screenSize"] = LUCID_GAL::Vector2(width, height);
	context[ "texelSize"] = LUCID_GAL::Vector2(1.f / width, 1.f / height);

	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 0, 0, 1), 1.f, 0x00);

	LUCID_GAL_PIPELINE.setRenderTarget(0, session->_selectTarget);
	LUCID_GAL_PIPELINE.setRenderTarget(1, session->_colorTarget);
	LUCID_GAL_PIPELINE.setRenderTarget(2, session->_glowTarget);
	LUCID_GAL_PIPELINE.updateTargets();

	session->_clear->render(session->_renderContext);
}

void State::blurGlowTarget(Session *session) const
{
	LUCID_GUI::Rectangle const &rectangle = session->_rectangle;

	float32_t  width = float32_t(rectangle.max.x - rectangle.min.x);
	float32_t height = float32_t(rectangle.max.y - rectangle.min.y);

	float32_t dx = 1.f / width;
	float32_t dy = 1.f / height;
	LUCID_GAL::Vector2 const horizontal = LUCID_GAL::Vector2(  dx, 0.f);
	LUCID_GAL::Vector2 const   vertical = LUCID_GAL::Vector2( 0.f,  dy);

	LUCID_GAL_PIPELINE.setRenderTarget(0, session->_blurTarget);
	LUCID_GAL_PIPELINE.setRenderTarget(1, nullptr);
	LUCID_GAL_PIPELINE.setRenderTarget(2, nullptr);
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(session->_blur, session->_blurParameters.theSource, session->_glowTarget);
	SET_MATERIAL_PARAMETER(session->_blur, session->_blurParameters.texelOffset, horizontal);
	session->_blur->render(session->_renderContext);

	LUCID_GAL_PIPELINE.setRenderTarget(0, session->_glowTarget);
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(session->_blur, session->_blurParameters.theSource, session->_blurTarget);
	SET_MATERIAL_PARAMETER(session->_blur, session->_blurParameters.texelOffset, vertical);
	session->_blur->render(session->_renderContext);
}

void State::postRender(Session *session) const
{
	blurGlowTarget(session);
	blurGlowTarget(session);

	LUCID_GAL_PIPELINE.restoreBackBuffer();
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(session->_post, session->_postParameters.colorTarget, session->_colorTarget);
	SET_MATERIAL_PARAMETER(session->_post,session-> _postParameters. glowTarget, session-> _glowTarget);
	session->_post->render(session->_renderContext);
}

/// 
/// 
/// 

void Stopped::onEnter(Session *session) const
{
}

void Stopped::onLeave(Session *session) const
{
}

Stopped const *Stopped::instance()
{
	thread_local static Stopped _instance;
	return &_instance;
}

/// 
/// 
/// 

void Starting::onEnter(Session *session) const
{
	gui::Rectangle const &rectangle = session->_rectangle;
	int32_t width = rectangle.max.x;
	int32_t height = rectangle.max.y;

	session->_renderContext = LUCID_GIGL::Context("content/render.context");

	session->_selectTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, width, height);
	session->_colorTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, width, height);
	session->_glowTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, width, height);
	session->_depthTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_FLOAT_R32, width, height);

	session->_blurTarget = LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, width, height);

	session->_selectReader = LUCID_GAL::TargetReader2D::create(session->_selectTarget, width, height);

	session->_clear = LUCID_GIGL::Mesh::create("content/clear.mesh");

	session->_blur = LUCID_GIGL::Mesh::create("content/blur.mesh");
	session->_blurParameters.texelOffset = GET_MATERIAL_PARAMETER(session->_blur, texelOffset);
	session->_blurParameters.  theSource = GET_MATERIAL_PARAMETER(session->_blur,   theSource);

	session->_post = LUCID_GIGL::Mesh::create("content/post.mesh");
	session->_postParameters.colorTarget = GET_MATERIAL_PARAMETER(session->_post, colorTarget);
	session->_postParameters. glowTarget = GET_MATERIAL_PARAMETER(session->_post,  glowTarget);

	auto btnHandler = std::bind(&Session::onButtonPress, session, std::placeholders::_1);
	auto cbxHandler = std::bind(&Session::onCheckboxPress, session, std::placeholders::_1);

	//
	//	setup the "configuring" user interface
	// 
	{
		gui::Panel *mainPanel = new gui::Panel(ID_NONE, gui::ANCHOR_FILL, gui::Size(width, height));

		gui::Panel *ctrlPanel = new gui::Panel(ID_NONE, gui::ANCHOR_SOUTH, gui::Size(128, 104));
		mainPanel->addChild(ctrlPanel);

		gui::Button *startButton = new gui::Button(ID_BTN_START, gui::ANCHOR_NORTH, gui::Size(42, 42), startTiles, btnHandler);
		ctrlPanel->addChild(startButton);
		startButton->setEnabled();

		session->_guiConfiguring = mainPanel;
		mainPanel->onEvent(gui::SizeEvent(rectangle));
	}

	//
	//	setup the "running" user interface
	//
	{
		gui::Panel *mainPanel = new gui::Panel(ID_NONE, gui::ANCHOR_FILL, gui::Size(width, height));
		
		session->_guiCurrentSelection = new gui::Label(ID_LBL_SELECT, gui::ANCHOR_SOUTH_WEST, gui::Size(512, 16), gui::ALIGNMENT::ALIGN_LEFT, "selection: <none>", gui::Color(0, 0.7f, 0, 1));
		mainPanel->addChild(session->_guiCurrentSelection);

		gui::Panel *mainSouthPanel = new gui::Panel(ID_NONE, gui::ANCHOR_SOUTH, gui::Size(168, 128));
		mainPanel->addChild(mainSouthPanel);
		
		gui::Panel *ctrlPanel = new gui::Panel(ID_NONE, gui::ANCHOR_NORTH, gui::Size(168, 58));
		mainSouthPanel->addChild(ctrlPanel);

		session->_guiTimeDisplay = new gui::Label(ID_LBL_TIME, gui::ANCHOR_NORTH, gui::Size(168, 16), gui::ALIGNMENT::ALIGN_CENTER, "-------.-----", gui::Color(0, 0.7f, 0, 1));
		ctrlPanel->addChild(session->_guiTimeDisplay);

		gui::Panel *ctrlSouthWestPanel = new gui::Panel(ID_NONE, gui::ANCHOR_SOUTH_WEST, gui::Size(84, 42));
		ctrlPanel->addChild(ctrlSouthWestPanel);

		gui::Panel *ctrlSouthEastPanel = new gui::Panel(ID_NONE, gui::ANCHOR_SOUTH_EAST, gui::Size(84, 42));
		ctrlPanel->addChild(ctrlSouthEastPanel);

		gui::Button *fastButton = new gui::Button(ID_BTN_FASTER, gui::ANCHOR_EAST, gui::Size(42, 42), fasterTiles, btnHandler);
		ctrlSouthEastPanel->addChild(fastButton);
		fastButton->setEnabled();

		gui::Button *stopButton = new gui::Button(ID_BTN_STOP, gui::ANCHOR_WEST, gui::Size(42, 42), stopTiles, btnHandler);
		ctrlSouthEastPanel->addChild(stopButton);
		stopButton->setEnabled();

		gui::Checkbox *playPauseButton = new gui::Checkbox(ID_BTN_PLAYPAUSE, gui::ANCHOR_EAST, gui::Size(42, 42), playPauseTiles, cbxHandler);
		ctrlSouthWestPanel->addChild(playPauseButton);
		playPauseButton->setEnabled();

		gui::Button *slowButton = new gui::Button(ID_BTN_SLOWER, gui::ANCHOR_WEST, gui::Size(42, 42), slowerTiles, btnHandler);
		ctrlSouthWestPanel->addChild(slowButton);
		slowButton->setEnabled();

		session->_guiRunning = mainPanel;
		mainPanel->onEvent(gui::SizeEvent(rectangle));
	}

	session->changeState(Configuring::instance());
}

void Starting::onLeave(Session *session) const
{
	// nop
}

Starting const *Starting::instance()
{
	thread_local static Starting _instance;
	return &_instance;
}

/// 
/// 
/// 

void Stopping::onEnter(Session *session) const
{
	/// TBD: shutdown everything...

	LUCID_CORE::reset_raw_ptr(session->_guiConfiguring);
	LUCID_CORE::reset_raw_ptr(session->_guiRunning);
	
	session->_postParameters = { nullptr, nullptr, };
	LUCID_CORE::reset_raw_ptr(session->_post);

	LUCID_CORE::reset_raw_ptr(session->_clear);

	session->_blurParameters = { nullptr, nullptr, };
	LUCID_CORE::reset_raw_ptr(session->_blur);

	LUCID_CORE::reset_raw_ptr(session->_blurTarget);

	LUCID_CORE::reset_raw_ptr(session->_selectTarget);
	LUCID_CORE::reset_raw_ptr(session->_colorTarget);
	LUCID_CORE::reset_raw_ptr(session->_glowTarget);
	LUCID_CORE::reset_raw_ptr(session->_depthTarget);

	LUCID_CORE::reset_raw_ptr(session->_selectReader);

	session->_renderContext = LUCID_GIGL::Context();

	session->changeState(Stopped::instance());
}

void Stopping::onLeave(Session *session) const
{
	// NOP
}

Stopping const *Stopping::instance()
{
	thread_local static Stopping _instance;
	return &_instance;
}

/// 
/// 
/// 

void Configuring::onEnter(Session *session) const
{
}

void Configuring::onLeave(Session *session) const
{
}
 
void Configuring::onEvent(Session *session, LUCID_GUI::TimerEvent const &event) const
{

}

void Configuring::onEvent(Session *session, gui::MouseEvent const &event) const
{
	session->_guiConfiguring->onEvent(event);
}

void Configuring::onButtonPress(Session *session, gui::Button *button) const
{
	if (ID_BTN_START == button->id())
		session->changeState(Running::instance());
}

void Configuring::render(Session *session, float64_t time) const
{
	preRender(session, time);

	session->_guiRender(session->_renderContext, session->_guiConfiguring);

	postRender(session);
}

Configuring const *Configuring::instance()
{
	thread_local static Configuring _instance;
	return &_instance;
}

/// 
/// 
/// 

void Running::onEnter(Session *session) const
{
	session->_simulator.initialize(2460707.500000);
}

void Running::onLeave(Session *session) const
{
}

void Running::onEvent(Session *session, LUCID_GUI::TimerEvent const &event) const
{
	session->_simulator.update(DAYS_PER_SECOND * event.dt);

	// test {
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(6) << session->_simulator.time();
	session->_guiTimeDisplay->text(stream.str());
	// } test
}

void Running::onEvent(Session *session, gui::MouseEvent const &event) const
{
	session->_guiRunning->onEvent(event);
}

void Running::onButtonPress(Session *session, gui::Button *button) const
{
	if (ID_BTN_STOP == button->id())
	{
		session->changeState(Configuring::instance());
	}
}

void Running::onCheckboxPress(Session *session, gui::Checkbox *button) const
{
	if (ID_BTN_PLAYPAUSE == button->id())
	{
		if (button->isChecked())
			session->_simulator.start();
		else
			session->_simulator.pause();
	}
}

void Running::render(Session *session, float64_t time) const
{
	preRender(session, time);

	// TBD: render simulation...

	session->_guiRender(session->_renderContext, session->_guiRunning);

	postRender(session);
}

Running const *Running::instance()
{
	thread_local static Running _instance;
	return &_instance;
}
