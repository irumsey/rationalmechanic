#pragma once

#include <memory>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

class Material;

LUCID_GIGL_END

namespace Lucid {
namespace GAL {

	ref class Program;

}	/// GAL
}	///	Lucid

namespace Lucid {
namespace GIGL {

	ref class Context;

	///	Material
	///
	///	simple wrapper about gigl::Material
	public ref class Material
	{
	public:
		Material(System::String ^path);

		~Material();

		!Material();

		void begin(Context ^context);

		void end();

		property GAL::Program ^program { GAL::Program ^get(); }

		property LUCID_GIGL::Material &ref { LUCID_GIGL::Material &get() { return *ptr; } }

		property LUCID_GIGL::Material *ptr { LUCID_GIGL::Material *get() { return _internal->get(); } }

	private:
		std::shared_ptr<LUCID_GIGL::Material> *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
