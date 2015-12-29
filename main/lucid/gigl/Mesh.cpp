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

	::lucid::gal::Pipeline::TOPOLOGY Mesh::topology() const
	{
		return _geometry->topology();
	}

	uint32_t Mesh::vertexCount() const
	{
		return _geometry->vertexCount();
	}

	uint32_t Mesh::indexCount() const
	{
		return _geometry->indexCount();
	}

	uint32_t Mesh::primitiveCount() const
	{
		return _geometry->primitiveCount();
	}

	void Mesh::render(Context const &context) const
	{
		_material->begin(context);
			draw();
		_material->end();
	}

	void Mesh::renderInstanced(Context const &context, int32_t count) const
	{
		_material->begin(context);
			drawInstanced(count);
		_material->end();
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
		_program.reset();
		_material.reset();
		_geometry.reset();
	}

}	///	gigl
}	///	lucid