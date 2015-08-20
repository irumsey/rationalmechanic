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

	class Program;

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

	///
	///
	///

	class Context;
	class Material;
	class Geometry;

	///	Mesh
	///
	///	a gigl mesh is defined as: a material and geometry.
	///
	///	note: notice the complete lack of any specific vertex
	///	structure.  mesh is entirely data driven which includes the
	///	vertex layout.  it would be improper to impose a vertex
	///	structure here.
	class Mesh final
	{
	public:
		Mesh(core::Reader &reader);

		virtual ~Mesh();

		core::Identity const &identity() const;

		std::shared_ptr<::lucid::gal::Program> program() const;

		std::shared_ptr<Material> material() const;

		void render(Context const &context) const;

		void draw() const;

		void drawInstanced(int32_t count) const;

		static Mesh *create(std::string const &path);

		static Mesh *create(core::Reader &reader);

	private:
		core::Identity const _identity;

		std::shared_ptr<::lucid::gal::Program> _program;

		std::shared_ptr<Material> _material;
		std::shared_ptr<Geometry> _geometry;

		void initialize(::lucid::core::Reader &reader);

		void shutdown();

		LUCID_PREVENT_COPY(Mesh);
		LUCID_PREVENT_ASSIGNMENT(Mesh);
	};

	inline core::Identity const &Mesh::identity() const
	{
		return _identity;
	}

	inline std::shared_ptr<::lucid::gal::Program> Mesh::program() const
	{
		return _program;
	}

	inline std::shared_ptr<Material> Mesh::material() const
	{
		return _material;
	}

}	///	gigl
}	///	lucid