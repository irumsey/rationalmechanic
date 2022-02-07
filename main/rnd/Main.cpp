///
///	Includes...
///
#include <windows.h>

#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Clock.h>
#include <lucid/core/Error.h>
#include <lucid/core/Types.h>
#include <sstream>

namespace core = ::lucid::core;
namespace  gal = ::lucid:: gal;
namespace math = ::lucid::math;

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

///	onUpdate
///
///
void onUpdate()
{
	LUCID_PROFILE_BEGIN("Update");

	// TBD: simulate stuff...

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
	pipeline.clear(true, true, true, gal::Color(0, 0, 0, 1), 1.f);

	// TBD: render stuff...

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
		break;

	default:
		break;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

#include <lucid/rm/Mind.h>
using Mind = ::lucid::rm::Mind;

///	WinMain
///
///
INT WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR cmdln, _In_ INT)
{
	LUCID_PROFILER_INITIALIZE();

	Mind mind(200, 1000);
	for (size_t i = 0; i < 1000; ++i)
		mind.update(10000);

	///
	///
	///
	core::Logger &coreLogger = core::Logger::instance();
	gal::System &galSystem = gal::System::instance();

	core::FileLog coreFileLog("results.log");
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

		galSystem.initialize(true, rect.right - rect.left, rect.bottom - rect.top, 1, hWindow);
		::ShowWindow(hWindow, SW_SHOW);

		///
		///	Get a clock and initialize the time(s)...
		///
		std::auto_ptr<core::Clock> wallClock(core::Clock::create());

		simTime = 0.f;

		wallTime = wallClock->time();
		frameInterpolant = 0.f;

		float64_t wallTimeLast = wallTime;
		float64_t wallTimeAccum = 0.f;

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
	}
	catch (core::Error const &)
	{
		exitCode = 1;
	}
	catch (...)
	{
		exitCode = 1;
	}

	///	Shutdown...
	galSystem.shutdown();

	///	Stop watching
	coreLogger.removeListener(&coreFileLog);

	///	Done
	LUCID_PROFILER_SHUTDOWN();

	return exitCode;
}
