#pragma once

#include <memory>

namespace lucid {
namespace gigl {

	class Geometry;

}	///	gigl
}	///	lucid

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

		property ::lucid::gigl::Geometry &ref { ::lucid::gigl::Geometry &get() { return *ptr; } }

		property ::lucid::gigl::Geometry *ptr { ::lucid::gigl::Geometry *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gigl::Geometry> *_internal = nullptr;

	};

} /// GIGL
}	///  Lucid
