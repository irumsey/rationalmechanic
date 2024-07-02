#pragma once

#include <memory>
#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class VertexFormat;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	///	VertexFormat
	///
	///
	public ref class VertexFormat
	{
	public:
		VertexFormat(System::String ^path);

		~VertexFormat();

		!VertexFormat();

		property LUCID_GAL::VertexFormat &ref { LUCID_GAL::VertexFormat &get() { return *ptr; } }

		property LUCID_GAL::VertexFormat *ptr { LUCID_GAL::VertexFormat *get() { return _internal->get(); } }

	private:
		std::shared_ptr<LUCID_GAL::VertexFormat> *_internal = nullptr;

	};

} /// GAL
} /// Lucid
