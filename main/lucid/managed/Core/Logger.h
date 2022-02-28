#pragma once

#include <cassert>
#include <msclr/marshal_cppstd.h>
#include <lucid/core/Logger.h>

namespace IS = System::Runtime::InteropServices;
namespace MI = msclr::interop;

namespace Lucid {
namespace Core {

	/// 
	/// 
	/// 
	public ref class Logger
	{
	public:
		ref class Listener abstract
		{
		public:
			//	a delegate type to pass to the unmanaged logger code for callbacks.
			delegate void delegate_type(std::string const &category, std::string const &message);

			Listener()
			{
				callbackDelegate = gcnew delegate_type(this, &Logger::Listener::onMessageDelegate);
			}

			~Listener()
			{
				///	GC NOTE: you can not call coreLogger.remove(...) here
				assert(nullptr == callback);
				this->!Listener();
			}

			!Listener()
			{
				///	GC NOTE: you can not call coreLogger.remove(...) here
				assert(nullptr == callback);
			}

			void Start()
			{
				::lucid::core::Logger &logger = ::lucid::core::Logger::instance();

				System::IntPtr ptr = IS::Marshal::GetFunctionPointerForDelegate(callbackDelegate);
				callback = new ::lucid::core::Logger::Callback(static_cast<::lucid::core::Logger::Callback::func_type>(ptr.ToPointer()));

				logger.addListener(callback);
			}

			void Stop()
			{
				::lucid::core::Logger &logger = ::lucid::core::Logger::instance();

				logger.removeListener(callback);

				delete callback;
				callback = nullptr;
			}

			virtual void OnMessage(System::String ^category, System::String ^message) = 0;

			void onMessageDelegate(std::string const &category, std::string const &message)
			{
				//	finally call the "managed" version...
				OnMessage(gcnew System::String(category.c_str()), gcnew System::String(message.c_str()));
			}

		private:
			///	keep callbackDelegate as long as callback...
			delegate_type ^callbackDelegate = nullptr;

			///	...or the gc will take it away and callback will crash
			::lucid::core::Logger::Callback *callback = nullptr;
		};

		static void Log(System::String ^category, System::String ^message)
		{
			::lucid::core::Logger &logger = ::lucid::core::Logger::instance();

			logger.log(MI::marshal_as<std::string>(category), MI::marshal_as<std::string>(message));
		}

	private:
		Logger()
		{
		}

	};

} /// Core
} /// Lucid
