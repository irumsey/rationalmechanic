#pragma once

#include <memory>
#include <string>
#include <vector>
#include <lucid/core/Identity.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Pipeline.h>

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

	class VertexFormat;
	class VertexBuffer;
	class IndexBuffer;

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gigl {

	///	Geometry
	///
	///
	class Geometry final
	{
	public:
		Geometry(::lucid::core::Reader &reader);

		virtual ~Geometry();

		core::Identity const &identity() const;

		void draw() const;

		void drawInstanced(int32_t count) const;

		static Geometry *create(std::string const &path);

		static Geometry *create(core::Reader &reader);

	private:
		core::Identity const _identity;

		::lucid::gal::Pipeline::TOPOLOGY _topology = gal::Pipeline::TOPOLOGY_TRIANGLELIST;

		std::unique_ptr<::lucid::gal::VertexFormat> _format;
		std::unique_ptr<::lucid::gal::VertexBuffer> _vertices;
		std::unique_ptr<::lucid::gal::IndexBuffer> _indices;

		void initialize(::lucid::core::Reader &reader);

		void shutdown();

		LUCID_PREVENT_COPY(Geometry);
		LUCID_PREVENT_ASSIGNMENT(Geometry);
	};

	inline core::Identity const &Geometry::identity() const
	{
		return _identity;
	}

}	///	gigl
}	///	lucid