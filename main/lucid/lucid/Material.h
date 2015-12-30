#pragma once

#include <memory>

namespace lucid {
namespace gigl  {

	class Material;

}	///	gigl
}	///	lucid

namespace lucid {

	ref class Context;
	ref class Program;

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

		property Program ^program { Program ^get(); }

		property ::lucid::gigl::Material &ref { ::lucid::gigl::Material &get() { return *ptr; } }

		property ::lucid::gigl::Material *ptr { ::lucid::gigl::Material *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gigl::Material> *_internal = nullptr;

	};

}	///  lucid
