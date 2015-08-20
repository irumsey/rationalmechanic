#include "Geometry.h"
#include "Resources.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/FileReader.h>

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

	Geometry *Geometry::create(std::string const &path)
	{
		return new Geometry(::lucid::core::FileReader(path));
	}

	Geometry *Geometry::create(core::Reader &reader)
	{
		return new Geometry(reader);
	}

	///
	///
	///

	Geometry::Geometry(core::Reader &reader)
	{
		initialize(reader);
	}

	Geometry::~Geometry()
	{
		shutdown();
	}

	void Geometry::draw() const
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.beginGeometry(_format.get());
			pipeline.setVertexStream(0, _vertices.get());
			pipeline.setIndexStream(_indices.get());
			pipeline.draw(_topology, _vertices->count(), _indices->count());
		pipeline.endGeometry(_format.get());
	}

	void Geometry::drawInstanced(int32_t count) const
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.beginGeometry(_format.get());
			pipeline.setVertexStream(0, _vertices.get());
			pipeline.setIndexStream(_indices.get());
			pipeline.drawInstanced(_topology, _vertices->count(), _indices->count(), count);
		pipeline.endGeometry(_format.get());
	}

	void Geometry::initialize(::lucid::core::Reader &reader)
	{
		reader.read(&_topology, sizeof(::lucid::gal::Pipeline::TOPOLOGY));

		_format.reset(::lucid::gal::VertexFormat::create(reader));
		_vertices.reset(::lucid::gal::VertexBuffer::create(reader));
		_indices.reset(::lucid::gal::IndexBuffer::create(reader));		
	}

	void Geometry::shutdown()
	{
		///	NOP for now
	}

}	///	gigl
}	///	lucid