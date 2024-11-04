#include "Geometry.h"
#include "Resources.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Profiler.h>

///
///
///

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

///
///
///

LUCID_GIGL_BEGIN

///
///	Geometry
///

Geometry::Geometry(LUCID_CORE::Reader &reader)
{
	initialize(reader);
}

Geometry::~Geometry()
{
	shutdown();
}

void Geometry::draw() const
{
	LUCID_GAL_PIPELINE.beginGeometry(_format.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertices.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indices.get());
		LUCID_GAL_PIPELINE.draw(_topology, _vertices->count(), _indices->count());
	LUCID_GAL_PIPELINE.endGeometry(_format.get());
}

void Geometry::drawInstanced(int32_t count) const
{
	LUCID_GAL_PIPELINE.beginGeometry(_format.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertices.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indices.get());
		LUCID_GAL_PIPELINE.drawInstanced(_topology, _vertices->count(), _indices->count(), int(count));
	LUCID_GAL_PIPELINE.endGeometry(_format.get());
}

void Geometry::initialize(LUCID_CORE::Reader &reader)
{
	reader.read(&_topology, sizeof(LUCID_GAL::Pipeline::TOPOLOGY));

    if (reader.read<bool>())
        _format.reset(LUCID_GAL::VertexFormat::create(reader.read<std::string>()));
    else
    	_format.reset(LUCID_GAL::VertexFormat::create(reader));

	_vertices.reset(LUCID_GAL::VertexBuffer::create(reader));
	_indices.reset(LUCID_GAL::IndexBuffer::create(reader));		
}

void Geometry::shutdown()
{
	_format.reset();
	_vertices.reset();
	_indices.reset();
}

uint32_t Geometry::vertexCount() const
{
	return _vertices->count();
}

uint32_t Geometry::indexCount() const
{
	return _indices->count();
}

Geometry *Geometry::create(std::string const &path)
{
	return Geometry::create(LUCID_CORE::FileReader(path));
}

Geometry *Geometry::create(LUCID_CORE::Reader &reader)
{
    return new Geometry(reader);
}

LUCID_GIGL_END