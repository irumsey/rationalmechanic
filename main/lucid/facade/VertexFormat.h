#pragma once

#include <memory>

namespace lucid {
namespace gal {

	class VertexFormat;

}	///	gal
}	///	lucid

namespace lucid {

	///	VertexFormat
	///
	///
	public ref class VertexFormat
	{
	public:
		VertexFormat(System::String ^path);

		~VertexFormat();

		!VertexFormat();

		property ::lucid::gal::VertexFormat &ref { ::lucid::gal::VertexFormat &get() { return *ptr; } }

		property ::lucid::gal::VertexFormat *ptr { ::lucid::gal::VertexFormat *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gal::VertexFormat> *_internal = nullptr;

	};

}	///  lucid
