#pragma once

#include <memory>

namespace lucid {
namespace gal  {

	class RenderTarget2D;

}	///	gal
}	///	lucid

namespace Lucid {
namespace GAL {

	///	
	///
	///
	public enum class RenderTargetFormat
	{
		UNDEFINED = -1,

		UINT_R8G8B8A8,
		UINT_R16G16,
		UINT_R10G10B10A2,
		FLOAT_R16G16,
		FLOAT_R32,
		FLOAT_R32G32,
		UINT_R32,
	};

	///	RenderTarget2D
	///
	///
	public ref class RenderTarget2D
	{
	public:
		typedef std::shared_ptr<::lucid::gal::RenderTarget2D> shared_ptr_t;

		RenderTarget2D(RenderTargetFormat format, int width, int height);

		~RenderTarget2D();

		!RenderTarget2D();

		property int width { int get(); }

		property int height { int get(); }

		property shared_ptr_t shared { shared_ptr_t get() { return *_internal; } }

		property ::lucid::gal::RenderTarget2D &ref { ::lucid::gal::RenderTarget2D &get() { return *ptr; } }

		property ::lucid::gal::RenderTarget2D *ptr { ::lucid::gal::RenderTarget2D *get() { return _internal->get(); } }

	private:
		shared_ptr_t *_internal = nullptr;

	};

} /// GAL
} /// Lucid
