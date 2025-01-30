#include "State.h"
#include "Session.h"
#include <lucid/gui/Button.h>
#include <lucid/gui/Label.h>
#include <lucid/gui/Panel.h>
#include <lucid/gui/Frame.h>
#include <lucid/gui/Events.h>
#include <lucid/gui/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>

namespace /* anonymous */ {

namespace gui = LUCID_GUI;

enum ID
{
	ID_NONE			=    -1,
		
	ID_BTN_START	= 0x101,

	ID_BTN_STOP		= 0x201,
	ID_BTN_SLOWER	= 0x202,
	ID_BTN_PLAY		= 0x203,
	ID_BTN_FASTER	= 0x204,
	ID_LBL_TIME		= 0x205,
	ID_LBL_SELECT	= 0x206,
};

// test {
gui::Button::Tiles const playTiles = {
	LUCID_GAL::Vector4( 0.f,   0.f, 96.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 0.f,  96.f, 96.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 0.f, 192.f, 96.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 0.f, 288.f, 96.f, 384.f) / 1024.f,
};
gui::Button::Tiles const pauseTiles = {
	LUCID_GAL::Vector4( 96.f,   0.f, 192.f,  96.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f,  96.f, 192.f, 192.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f, 192.f, 192.f, 288.f) / 1024.f,
	LUCID_GAL::Vector4( 96.f, 288.f, 192.f, 384.f) / 1024.f,
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

void setup(LUCID_GIGL::Context &context)
{
	float32_t  width = float32_t(LUCID_GAL_SYSTEM. width());
	float32_t height = float32_t(LUCID_GAL_SYSTEM.height());

	context["screenSize"] = LUCID_GAL::Vector2(width, height);
	context["texelSize"] = LUCID_GAL::Vector2(1.f / width, 1.f / height);
}

}	// anonymous

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

	auto handler = std::bind(&Session::onButtonPress, session, std::placeholders::_1);

	//
	//	setup the "configuring" user interface
	// 
	{
		gui::Panel *mainPanel = new gui::Panel(ID_NONE, gui::ANCHOR_FILL, width, height);

		gui::Button *startButton = new gui::Button(ID_BTN_START, gui::ANCHOR_SOUTH, 42, 42, handler, playTiles);
		mainPanel->addChild(startButton);
		startButton->enable();

		session->_guiConfiguring = mainPanel;
		mainPanel->onEvent(gui::SizeEvent(rectangle));
	}

	//
	//	setup the "running" user interface
	//
	{
		gui::Panel *mainPanel = new gui::Panel(ID_NONE, gui::ANCHOR_FILL, width, height);
		
		gui::Label *timeLabel = new gui::Label(ID_LBL_TIME, gui::ANCHOR_SOUTH_WEST, 128, 16);
		mainPanel->addChild(timeLabel);

		gui::Label *slctLabel = new gui::Label(ID_LBL_SELECT, gui::ANCHOR_SOUTH_EAST, 128, 16);
		mainPanel->addChild(slctLabel);

		gui::Panel *mainSouthPanel = new gui::Panel(ID_NONE, gui::ANCHOR_SOUTH, 168, 128);
		mainPanel->addChild(mainSouthPanel);
		
		gui::Panel *ctrlWestPanel = new gui::Panel(ID_NONE, gui::ANCHOR_WEST, 84, 128);
		mainSouthPanel->addChild(ctrlWestPanel);

		gui::Panel *ctrlEastPanel = new gui::Panel(ID_NONE, gui::ANCHOR_EAST, 84, 128);
		mainSouthPanel->addChild(ctrlEastPanel);

		gui::Button *fastButton = new gui::Button(ID_BTN_FASTER, gui::ANCHOR_EAST, 42, 42, handler, fasterTiles);
		ctrlEastPanel->addChild(fastButton);
		fastButton->enable();

		gui::Button *stopButton = new gui::Button(ID_BTN_STOP, gui::ANCHOR_WEST, 42, 42, handler, stopTiles);
		ctrlEastPanel->addChild(stopButton);
		stopButton->enable();

		gui::Button *playButton = new gui::Button(ID_BTN_PLAY, gui::ANCHOR_EAST, 42, 42, handler, playTiles);
		ctrlWestPanel->addChild(playButton);
		playButton->enable();

		gui::Button *slowButton = new gui::Button(ID_BTN_SLOWER, gui::ANCHOR_WEST, 42, 42, handler, slowerTiles);
		ctrlWestPanel->addChild(slowButton);
		slowButton->enable();

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

	::safe_delete(session->_guiConfiguring);
	::safe_delete(session->_guiRunning);
	
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

void Configuring::onEvent(Session *session, gui::SizeEvent const &event) const
{
	session->_guiConfiguring->onEvent(event);
	session->_guiRunning->onEvent(event);
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

void Configuring::update(Session *session, float64_t t, float32_t dt) const
{
}

void Configuring::render(Session *session, float64_t t, float32_t interpolant) const
{
	::setup(session->_renderContext);

	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1), 1.f, 0x00);
	session->_guiRender(session->_renderContext, session->_guiConfiguring);
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
}

void Running::onLeave(Session *session) const
{
}

void Running::onEvent(Session *session, gui::SizeEvent const &event) const
{
	session->_guiConfiguring->onEvent(event);
	session->_guiRunning->onEvent(event);
}

void Running::onEvent(Session *session, gui::MouseEvent const &event) const
{
	session->_guiRunning->onEvent(event);
}

void Running::onButtonPress(Session *session, gui::Button *button) const
{
	if (ID_BTN_STOP == button->id())
		session->changeState(Configuring::instance());
}

void Running::update(Session *session, float64_t t, float32_t dt) const
{
}

void Running::render(Session *session, float64_t t, float32_t interpolant) const
{
	::setup(session->_renderContext);

	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 0, 0, 1), 1.f, 0x00);
	session->_guiRender(session->_renderContext, session->_guiRunning);
}

Running const *Running::instance()
{
	thread_local static Running _instance;
	return &_instance;
}
