///
///	Includes...
///
#include <windows.h>

#include <sstream>
#include "Utility.h"
#include "UserInput.h"
#include "SessionState.h"
#include "Session.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/core/Clock.h>
#include <lucid/core/Profiler.h>

///
///	Constants...
///
int32_t   const SCREEN_WIDTH = 1280;
int32_t   const SCREEN_HEIGHT = 720;
float64_t const TIME_STEP = 0.1;
float64_t const TIME_LIMIT = 0.3;

///
///	Globals (sigh)...
///
int32_t   frameCount = 0;
float64_t simTime = 0;
float64_t wallTime = 0;
float32_t frameInterpolant = 0;

bool mouseLeftDown = false;
bool mouseRightDown = false;

Session session;

///
///
///

#ifndef HID_USAGE_PAGE_GENERIC
#	define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#	define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

void registerMouseDevice(HWND hWindow)
{
	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = hWindow;
	::RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE));
}

void handleMouseInput(HRAWINPUT hInput)
{
	UINT size = sizeof(RAWINPUT);
	RAWINPUT rawInput;

	::GetRawInputData(hInput, RID_INPUT, &rawInput, &size, sizeof(RAWINPUTHEADER));

	RAWINPUTHEADER const &header = rawInput.header;
	if (RIM_TYPEMOUSE == header.dwType)
	{
		RAWMOUSE const &mouse = rawInput.data.mouse;

		mouseLeftDown  = ( RI_MOUSE_LEFT_BUTTON_DOWN & mouse.ulButtons) ?  true :  mouseLeftDown;
		mouseLeftDown  = (   RI_MOUSE_LEFT_BUTTON_UP & mouse.ulButtons) ? false :  mouseLeftDown;
		mouseRightDown = (RI_MOUSE_RIGHT_BUTTON_DOWN & mouse.ulButtons) ?  true : mouseRightDown;
		mouseRightDown = (  RI_MOUSE_RIGHT_BUTTON_UP & mouse.ulButtons) ? false : mouseRightDown;

		MouseEvent event;
		event.x = mouse.lLastX;
		event.y = mouse.lLastY;
		event.btnDownLeft = mouseLeftDown;
		event.btnDownRight = mouseRightDown;

		session.onInput(event);
	}
}

///	onUpdate
///
///
///	Note: time (t) and elapsed time (dt) should be used for all game simulation
///	(ie no part of the game should use its own clock.)
void onUpdate()
{
	LUCID_PROFILE_BEGIN("Update");

	session.onUpdate(simTime, TIME_STEP);

	LUCID_PROFILE_END();
}

///	onRender
///
///
void onRender()
{
	LUCID_PROFILE_BEGIN("Render");

	lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	pipeline.beginScene();
	session.onRender((float32_t)wallTime, frameInterpolant);
	pipeline.endScene();

	LUCID_PROFILE_END();
}

///	onMessage
///	
///
LRESULT WINAPI onMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
		break;

	case WM_PAINT:
		::onRender();
		::ValidateRect(hWnd, 0);
		return 0;
		break;

	case WM_KEYDOWN:
		if (VK_ESCAPE == wParam)
		{
			::PostQuitMessage(0);
			return 0;
		}
		break;

	case WM_INPUT:
		::handleMouseInput((HRAWINPUT)lParam);
		break;

	default:
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


///	WinMain
///
///
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdln, INT)
{
	LUCID_PROFILER_INITIALIZE();

	///
	///
	///
	lucid::core::Logger &coreLogger = lucid::core::Logger::instance();
	lucid::gal::System &galSystem = lucid::gal::System::instance();

	lucid::core::FileLog coreFileLog("results.log");
	coreLogger.addListener(&coreFileLog);

	INT exitCode = 0;

	try
	{
		///
		///	Create the window
		///
		WNDCLASSEX winClass =
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			::onMessage,
			0,
			0,
			::GetModuleHandle(0),
			0,
			0,
			0,
			0,
			L"DAD",
			0,
		};
		::RegisterClassEx(&winClass);

		HWND hWindow = ::CreateWindow
			(
			L"DAD",
			L"DAD!",
			WS_OVERLAPPEDWINDOW,
			0, 16,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			::GetDesktopWindow(),
			0,
			winClass.hInstance,
			0
			);

		///
		///	Initialize...
		///
		RECT rect;
		::GetClientRect(hWindow, &rect);

		galSystem.initialize(true, rect.right - rect.left, rect.bottom - rect.top, 2, hWindow);

		::ShowWindow(hWindow, SW_SHOW);

		::registerMouseDevice(hWindow);

		///
		///	Get a clock and initialize the time(s)...
		///
		std::auto_ptr<lucid::core::Clock> wallClock(lucid::core::Clock::create());

		simTime = 0.f;

		wallTime = wallClock->time();
		frameInterpolant = 0.f;

		float64_t wallTimeLast = wallTime;
		float64_t wallTimeAccum = 0.f;

		///
		///	Start the session...
		///
		session.setState(SessionStarting::instance());

		///
		///	Message pump
		///
		MSG msg;
		::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
		while (WM_QUIT != msg.message)
		{
			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				LUCID_PROFILE_BEGIN("Frame");

				wallTime = wallClock->time();
				float64_t wallTimeElapsed = wallTime - wallTimeLast;
				wallTimeLast = wallTime;

				wallTimeElapsed = (wallTimeElapsed > TIME_LIMIT) ? TIME_LIMIT: wallTimeElapsed;
				wallTimeAccum += wallTimeElapsed;
				while (wallTimeAccum >= TIME_STEP)
				{
					simTime += TIME_STEP;
					::onUpdate();
					wallTimeAccum -= TIME_STEP;
				}
				frameInterpolant = (float32_t)(wallTimeAccum / TIME_STEP);

				::onRender();

				LUCID_PROFILE_END();
			}
		}

		::dumpProfileData("profile.log");

		exitCode = session.passed() ? 0 : 1;
	}
	catch (lucid::core::Error const &error)
	{
		::log("ERROR", error.what());
		exitCode = 1;
	}
	catch (...)
	{
		::log("ERROR", "unknown exception");
		exitCode = 1;
	}

	///	shutdown session
	session.setState(SessionStopping::instance());

	///	Shutdown...
	galSystem.shutdown();

	///	Stop watching
	coreLogger.removeListener(&coreFileLog);

	///	Done
	LUCID_PROFILER_SHUTDOWN();

	return exitCode;
}
