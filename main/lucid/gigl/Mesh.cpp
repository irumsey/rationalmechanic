#include "Mesh.h"
#include "Geometry.h"
#include "Material.h"
#include "Resources.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>

///
///
///

namespace gal = ::lucid::gal;

///
///
///
namespace lucid {
namespace gigl {

	///
	///
	///

	Mesh *Mesh::create(std::string const &path)
	{
		return new Mesh(::lucid::core::FileReader(path));
	}

	Mesh *Mesh::create(core::Reader &reader)
	{
		return new Mesh(reader);
	}

	///
	///
	///

	Mesh::Mesh(core::Reader &reader)
	{
		initialize(reader);
	}

	Mesh::~Mesh()
	{
		shutdown();
	}

	void Mesh::render(Context const &context) const
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.beginProgram(_program.get());
			_material->begin(context);
				draw();
			_material->end();
		pipeline.endProgram(_program.get());
	}

	void Mesh::draw() const
	{
		_geometry->draw();
	}

	void Mesh::drawInstanced(int32_t count) const
	{
		_geometry->drawInstanced(count);
	}

	void Mesh::initialize(::lucid::core::Reader &reader)
	{
		if (reader.read<bool>())
		{
			_material = Resources::get<Material>(reader.read<std::string>());
		}
		else
		{
			_material.reset(Material::create(reader));
		}
		
		if (reader.read<bool>())
		{
			_geometry = Resources::get<Geometry>(reader.read<std::string>());
		}
		else
		{
			_geometry.reset(Geometry::create(reader));
		}

		_program = _material->program();
	}

	void Mesh::shutdown()
	{
		///	NOP for now
	}

}	///	gigl
}	///	lucid