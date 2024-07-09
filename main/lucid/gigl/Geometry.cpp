#include "Geometry.h"
#include "Resources.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/FileReader.h>

///
///
///
LUCID_GIGL_BEGIN

///
///
///

Geometry *Geometry::create(std::string const &path)
{
	return new Geometry(LUCID_CORE::FileReader(path));
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

uint32_t Geometry::vertexCount() const
{
	return _vertices->count();
}

uint32_t Geometry::indexCount() const
{
	return _indices->count();
}

uint32_t Geometry::primitiveCount() const
{
	switch (_topology)
	{
	case LUCID_GAL::Pipeline::TOPOLOGY_POINT_LIST:
		return indexCount();
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_LINE_STRIP:
		return indexCount() - 1;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_LINE_LIST:
		return indexCount() / 2;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_STRIP:
		return indexCount() - 2;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_LIST:
		return indexCount() / 3;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_ADJACENCY:
		return indexCount() / 6;
		break;
	default:
		break;
	}

	return 0;
}

void Geometry::draw() const
{
	LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

	pipeline.beginGeometry(_format.get());
		pipeline.setVertexStream(0, _vertices.get());
		pipeline.setIndexStream(_indices.get());
		pipeline.draw(_topology, _vertices->count(), _indices->count());
	pipeline.endGeometry(_format.get());
}

void Geometry::drawInstanced(int32_t count) const
{
	LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

	pipeline.beginGeometry(_format.get());
		pipeline.setVertexStream(0, _vertices.get());
		pipeline.setIndexStream(_indices.get());
		pipeline.drawInstanced(_topology, _vertices->count(), _indices->count(), count);
	pipeline.endGeometry(_format.get());
}

void Geometry::initialize(LUCID_CORE::Reader &reader)
{
	reader.read(&_topology, sizeof(LUCID_GAL::Pipeline::TOPOLOGY));

	_format.reset(LUCID_GAL::VertexFormat::create(reader));
	_vertices.reset(LUCID_GAL::VertexBuffer::create(reader));
	_indices.reset(LUCID_GAL::IndexBuffer::create(reader));		
}

void Geometry::shutdown()
{
	///	NOP for now
}

LUCID_GIGL_END