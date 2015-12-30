#pragma once

#include <memory>
#include <lucid/math/AABB.h>
#include <lucid/gal/Types.h>
#include <lucid/gal/Pipeline.h>

namespace lucid {
namespace gigl  {

	class Mesh;

}	///	gigl
}	///	lucid

namespace lucid {

	ref class Context;

	///	Mesh
	///
	///	simple wrapper about gigl::Mesh
	public ref class Mesh
	{
	public:
		Mesh(System::String ^path);

		~Mesh();

		!Mesh();

		void render(Context ^context);

		property ::lucid::gigl::Mesh &ref { ::lucid::gigl::Mesh &get() { return *ptr; } }

		property ::lucid::gigl::Mesh *ptr { ::lucid::gigl::Mesh *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gigl::Mesh> *_internal = nullptr;

	};

}	///  lucid
