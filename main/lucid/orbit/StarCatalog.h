#pragma once

#include <memory>
#include <string>
#include <vector>
#include <lucid/core/Noncopyable.h>

namespace lucid {
namespace gal {

	class VertexBuffer;

}	///	gal
}	///	lucid

namespace lucid {
namespace gigl {

	class Context;
	class Mesh;

}	///	gigl
}	///	lucid

namespace lucid {
namespace orbit {

	///	StarCatalog
	///
	///	The order of the stars is the order from the
	///	Yale Bright Star Catalog BSC5.
	class StarCatalog
	{
	public:
		///
		///
		///
		struct Entry
		{
			size_t xno = 0;
			std::string type;
			double right_ascension = 0.0;
			double declination = 0.0;
			float magnitude = 0.f;
		};
		typedef std::vector<Entry> ordinal_vec_t;
		typedef ordinal_vec_t::const_iterator Iterator;

		StarCatalog() = default;

		StarCatalog(std::string const &path);

		virtual ~StarCatalog() = default;

		void initialize(std::string const &path);

		void shutdown();

		Iterator begin() const;

		Iterator end() const;

		void render(lucid::gigl::Context const &context) const;

	private:
		ordinal_vec_t _ordinal;

		std::shared_ptr<gal::VertexBuffer> _instances;
		std::shared_ptr<gigl::Mesh> _mesh;

		LUCID_PREVENT_COPY(StarCatalog);
		LUCID_PREVENT_ASSIGNMENT(StarCatalog);
	};

	inline StarCatalog::Iterator StarCatalog::begin() const
	{
		return _ordinal.begin();
	}

	inline StarCatalog::Iterator StarCatalog::end() const
	{
		return _ordinal.end();
	}

}	///	orbit
}	///	lucid