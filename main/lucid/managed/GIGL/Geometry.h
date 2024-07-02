#pragma once

#include <memory>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

class Geometry;

LUCID_GIGL_END

namespace Lucid {
namespace GIGL {

	///	Geometry
	///
	///
	public ref class Geometry
	{
	public:
		Geometry(System::String ^path);

		~Geometry();

		!Geometry();

		void draw();

		void drawInstanced(int count);

		property LUCID_GIGL::Geometry &ref { LUCID_GIGL::Geometry &get() { return *ptr; } }

		property LUCID_GIGL::Geometry *ptr { LUCID_GIGL::Geometry *get() { return _internal->get(); } }

	private:
		std::shared_ptr<LUCID_GIGL::Geometry> *_internal = nullptr;

	};

} /// GIGL
}	///  Lucid
