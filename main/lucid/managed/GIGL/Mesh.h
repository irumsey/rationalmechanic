#pragma once

#include <memory>
#include <lucid/math/AABB.h>
#include <lucid/gal/Types.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

class Mesh;

LUCID_GIGL_END

namespace Lucid {
namespace GIGL {

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

		property LUCID_GIGL::Mesh &ref { LUCID_GIGL::Mesh &get() { return *ptr; } }

		property LUCID_GIGL::Mesh *ptr { LUCID_GIGL::Mesh *get() { return _internal->get(); } }

	private:
		std::shared_ptr<LUCID_GIGL::Mesh> *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
