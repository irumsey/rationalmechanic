#pragma once

#include <memory>

namespace lucid {
namespace gigl  {

	class Material;

}	///	gigl
}	///	lucid

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

		property ::lucid::gigl::Material &ref { ::lucid::gigl::Material &get() { return *ptr; } }

		property ::lucid::gigl::Material *ptr { ::lucid::gigl::Material *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gigl::Material> *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
