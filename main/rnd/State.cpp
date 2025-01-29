#include "State.h"
#include "Session.h"
#include <lucid/gui/Button.h>
#include <lucid/gui/Label.h>
#include <lucid/gui/Panel.h>
#include <lucid/gui/Frame.h>
#include <lucid/gui/Events.h>
#include <lucid/gui/Types.h>
#include <lucid/gal/Pipeline.h>

namespace /* anonymous */ {

	enum CID
	{
		CID_NONE		=    -1,
		
		CID_BTN_START	= 0x101,

		CID_BTN_STOP	= 0x201,
		CID_BTN_SLOWER	= 0x202,
		CID_BTN_PLAY	= 0x203,
		CID_BTN_FASTER	= 0x204,
		CID_LBL_TIME	= 0x205,
		CID_LBL_SELECT	= 0x206,
	};

};

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
	LUCID_GUI::Rectangle const &rectangle = session->_rectangle;
	int32_t width = rectangle.max.x;
	int32_t height = rectangle.max.y;

	// test {
	LUCID_GUI::Button::Tiles tiles = {
		LUCID_GAL::Vector4(0.f, 0.f, 1.f, 1.f),
		LUCID_GAL::Vector4(0.f, 0.f, 1.f, 1.f),
		LUCID_GAL::Vector4(0.f, 0.f, 1.f, 1.f),
		LUCID_GAL::Vector4(0.f, 0.f, 1.f, 1.f),
	};
	// } test

	//
	//	setup the "configuring" user interface
	// 
	{
		LUCID_GUI::Panel *mainPanel = new LUCID_GUI::Panel(CID_NONE, LUCID_GUI::Frame::ANCHOR_FILL, width, height);

		LUCID_GUI::Button *startButton = new LUCID_GUI::Button(CID_BTN_START, LUCID_GUI::Frame::ANCHOR_SOUTH, 128, 128, tiles);
		mainPanel->addChild(startButton);
		startButton->set(std::bind(&Session::onButtonPress, session, std::placeholders::_1));
		startButton->enable();

		session->_guiConfiguring = mainPanel;
		mainPanel->onEvent(LUCID_GUI::SizeEvent(rectangle));
	}

	//
	//	setup the "running" user interface
	//
	{
		LUCID_GUI::Panel *mainPanel = new LUCID_GUI::Panel(CID_NONE, LUCID_GUI::Frame::ANCHOR_FILL, width, height);
		
		LUCID_GUI::Button *stopButton = new LUCID_GUI::Button(CID_BTN_STOP, LUCID_GUI::Frame::ANCHOR_NORTH_EAST, 64, 64, tiles);
		mainPanel->addChild(stopButton);
		stopButton->set(std::bind(&Session::onButtonPress, session, std::placeholders::_1));
		stopButton->enable();

		LUCID_GUI::Label *slctLabel = new LUCID_GUI::Label(CID_LBL_SELECT, LUCID_GUI::Frame::ANCHOR_SOUTH_EAST, 128, 16);
		mainPanel->addChild(slctLabel);

		LUCID_GUI::Panel *ctrlPanel = new LUCID_GUI::Panel(CID_NONE, LUCID_GUI::Frame::ANCHOR_SOUTH, 256, 64);
		mainPanel->addChild(ctrlPanel);
		
		LUCID_GUI::Label *timeLabel = new LUCID_GUI::Label(CID_LBL_TIME, LUCID_GUI::Frame::ANCHOR_SOUTH_WEST, 128, 16);
		mainPanel->addChild(timeLabel);
		
		LUCID_GUI::Button *fastButton = new LUCID_GUI::Button(CID_BTN_FASTER, LUCID_GUI::Frame::ANCHOR_EAST, 64, 64, tiles);
		ctrlPanel->addChild(fastButton);
		fastButton->set(std::bind(&Session::onButtonPress, session, std::placeholders::_1));
		fastButton->enable();

		LUCID_GUI::Button *playButton = new LUCID_GUI::Button(CID_BTN_PLAY, LUCID_GUI::Frame::ANCHOR_SOUTH, 128, 64, tiles);
		ctrlPanel->addChild(playButton);
		playButton->set(std::bind(&Session::onButtonPress, session, std::placeholders::_1));
		playButton->enable();

		LUCID_GUI::Button *slowButton = new LUCID_GUI::Button(CID_BTN_SLOWER, LUCID_GUI::Frame::ANCHOR_WEST, 64, 64, tiles);
		ctrlPanel->addChild(slowButton);
		slowButton->set(std::bind(&Session::onButtonPress, session, std::placeholders::_1));
		slowButton->enable();

		session->_guiRunning = mainPanel;
		mainPanel->onEvent(LUCID_GUI::SizeEvent(rectangle));
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

void Configuring::onEvent(Session *session, LUCID_GUI::SizeEvent const &event) const
{
	session->_guiConfiguring->onEvent(event);
	session->_guiRunning->onEvent(event);
}

void Configuring::onEvent(Session *session, LUCID_GUI::MouseEvent const &event) const
{
	session->_guiConfiguring->onEvent(event);
}

void Configuring::onButtonPress(Session *session, LUCID_GUI::Button *button) const
{
	if (CID_BTN_START == button->id())
		session->changeState(Running::instance());
}

void Configuring::update(Session *session, float64_t t, float32_t dt) const
{
}

void Configuring::render(Session *session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1), 1.f, 0x00);
	session->_guiRender(session->_guiConfiguring);
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

void Running::onEvent(Session *session, LUCID_GUI::SizeEvent const &event) const
{
	session->_guiConfiguring->onEvent(event);
	session->_guiRunning->onEvent(event);
}

void Running::onEvent(Session *session, LUCID_GUI::MouseEvent const &event) const
{
	session->_guiRunning->onEvent(event);
}

void Running::onButtonPress(Session *session, LUCID_GUI::Button *button) const
{
	if (CID_BTN_STOP == button->id())
		session->changeState(Configuring::instance());
}

void Running::update(Session *session, float64_t t, float32_t dt) const
{
}

void Running::render(Session *session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 0, 0, 1), 1.f, 0x00);
	session->_guiRender(session->_guiRunning);
}

Running const *Running::instance()
{
	thread_local static Running _instance;
	return &_instance;
}
