///
///	Includes...
///
#include <windows.h>
#include <windowsx.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Clock.h>
#include <lucid/core/Error.h>
#include <lucid/core/Types.h>
#include <sstream>
#include "Session.h"
#include "State.h"
#include "Utility.h"

/// 
/// 
/// 
LUCID_ANONYMOUS_BEGIN

///
///	Constants...
///	TBD: move to Session
int32_t   const  SCREEN_WIDTH = 1280;
int32_t   const SCREEN_HEIGHT =  720;
float32_t const     TIME_STEP = 0.1f;
float64_t const    TIME_LIMIT = 0.30;

///
///	Globals...
///	TBD: move to Session
///      the issue is the message pump and sim loop
///      which will require moving as well.
int32_t         frameCount = 0;
float64_t          simTime = 0;
float64_t         wallTime = 0;
float32_t frameInterpolant = 0;

Session session;

LUCID_ANONYMOUS_END

///	onUpdate
///
///
void onUpdate()
{
	LUCID_PROFILE_BEGIN("Update");
	session.update(simTime, TIME_STEP);
	LUCID_PROFILE_END();
}

///	onRender
///
///
void onRender()
{
	LUCID_PROFILE_BEGIN("Render");

	LUCID_GAL_PIPELINE.beginScene();
	session.render(wallTime, frameInterpolant);
	LUCID_GAL_PIPELINE.endScene();

	LUCID_PROFILE_END();
}

///
/// 
/// 
void onMouseMiddleButton(bool dwn, point2d_t const &pnt)
{
	LUCID_PROFILE_BEGIN("Middle Mouse Button");
	session.onMouseButton(MOUSE_BUTTON_MIDDLE, dwn, pnt);
	LUCID_PROFILE_END();
}

void onMouseWheel(int32_t delta)
{
	LUCID_PROFILE_BEGIN("Mouse Wheel");
	session.onMouseWheel(delta);
	LUCID_PROFILE_END();
}

void onMouseMove(point2d_t const &point)
{
	LUCID_PROFILE_BEGIN("Mouse Move");
	session.onMouseMove(point);
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
		session.shutdown();
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
			session.shutdown();
			::PostQuitMessage(0);
			return 0;
		}
		break;

	case WM_MBUTTONDOWN:
		::onMouseMiddleButton(true, point2d_t(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;

	case WM_MBUTTONUP:
		::onMouseMiddleButton(false, point2d_t(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;

	case WM_MOUSEWHEEL:
		::onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;

	case WM_MOUSEMOVE:
		::onMouseMove(point2d_t(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;

	case WM_INPUT:
		break;

	default:
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

///	WinMain
///
///
INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR cmdln, _In_ INT)
{
	LUCID_PROFILER_INITIALIZE();

	///
	///
	///
	LUCID_CORE::Logger &coreLogger = LUCID_CORE::Logger::instance();

	LUCID_CORE::FileLog coreFileLog("results.log");
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
			L"RND",
			0,
		};
		::RegisterClassEx(&winClass);

		HWND hWindow = ::CreateWindow
			(
				L"RND",
				L"R&D!",
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

		LUCID_GAL_SYSTEM.initialize(true, rect.right - rect.left, rect.bottom - rect.top, 1, hWindow);
		::ShowWindow(hWindow, SW_SHOW);

		///
		///	Get a clock and initialize the time(s)...
		///
		std::auto_ptr<LUCID_CORE::Clock> wallClock(LUCID_CORE::Clock::create());

		simTime = 0.f;

		wallTime = wallClock->time();
		frameInterpolant = 0.f;

		float64_t wallTimeLast = wallTime;
		float64_t wallTimeAccum = 0.f;

		session.initialize();

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

		session.shutdown();

		::dumpProfileData("profile.log");
	}
	catch (LUCID_CORE::Error const &error)
	{
		log("ERR", error.what());
		exitCode = 1;
	}
	catch (...)
	{
		log("ERR", "unknown error");
		exitCode = 1;
	}

	///	Shutdown...
	LUCID_GAL_SYSTEM.shutdown();

	///	Stop watching
	coreLogger.removeListener(&coreFileLog);

	///	Done
	LUCID_PROFILER_SHUTDOWN();

	return exitCode;
}
