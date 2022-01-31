#pragma once

#include <string>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

///
///
///
namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace gal {

	///	VertexElement
	///
	///	describes one element/entry in a vertex format
	struct VertexElement
	{
		enum FORMAT
		{
			FORMAT_UNDEFINED = -1,

			FORMAT_FLOAT1 = 0,
			FORMAT_FLOAT2,
			FORMAT_FLOAT3,
			FORMAT_FLOAT4,

			FORMAT_UINT1,
			FORMAT_UINT2,
			FORMAT_UINT3,
			FORMAT_UINT4,
		};

		enum TYPE
		{
			TYPE_UNDEFINED = -1,

			TYPE_VERTEX = 0,
			TYPE_INSTANCE,
		};

		FORMAT format;
		TYPE type;

		int32_t location;
		int32_t stream;
		int32_t offset;
	};

	///	VertexFormat
	///
	///	describes the format/layout of a vertex bridging the requirements
	///	of the vertex shader and the actual vertex supplied.
	///
	///	note: the only requirement imposed upon the "actual" vertex is
	///	that it must supply, at minimum, that which is required by the
	///	vertex shader.  in other words, the actual vertex may be a superset
	///	of that required by the vertex shader.
	class VertexFormat
	{
	public:
		virtual ~VertexFormat() = 0 {}

		virtual std::vector<VertexElement> const &layout() const = 0;

		static VertexFormat *create(VertexElement const *layout, int32_t count);

		static VertexFormat *create(std::vector<VertexElement> const &layout);

		static VertexFormat *create(std::string const &path);

		static VertexFormat *create(::lucid::core::Reader &reader);

	protected:
		VertexFormat() {}

		LUCID_PREVENT_COPY(VertexFormat);
		LUCID_PREVENT_ASSIGNMENT(VertexFormat);
	};

}	///	gal
}	///	lucid