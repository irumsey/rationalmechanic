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
///	Geometry
///

typedef Geometry *(*creator_func_t)(LUCID_CORE::Reader &reader);

Geometry *createFixedGeometry(LUCID_CORE::Reader &reader)
{
    return new FixedGeometry(reader);
}

Geometry *createAdaptiveGeometry(LUCID_CORE::Reader &reader)
{
    return new AdaptiveGeometry(reader);
}

creator_func_t const createGeometry[2] =
{
    createFixedGeometry,
    createAdaptiveGeometry,
};

Geometry *Geometry::create(std::string const &path)
{
	return Geometry::create(LUCID_CORE::FileReader(path));
}

Geometry *Geometry::create(LUCID_CORE::Reader &reader)
{
    uint32_t kind = reader.read<uint32_t>();
    LUCID_VALIDATE(kind < 2, "invalid geomtry type specified");

    return createGeometry[kind](reader);
}

///
///	FixedGeometry
///

FixedGeometry::FixedGeometry(LUCID_CORE::Reader &reader)
{
	initialize(reader);
}

FixedGeometry::~FixedGeometry()
{
	shutdown();
}

uint32_t FixedGeometry::vertexCount() const
{
	return _vertices->count();
}

uint32_t FixedGeometry::indexCount() const
{
	return _indices->count();
}

void FixedGeometry::draw() const
{
	LUCID_GAL_PIPELINE.beginGeometry(_format.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertices.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indices.get());
		LUCID_GAL_PIPELINE.draw(_topology, _vertices->count(), _indices->count());
	LUCID_GAL_PIPELINE.endGeometry(_format.get());
}

void FixedGeometry::drawInstanced(int32_t count) const
{
	LUCID_GAL_PIPELINE.beginGeometry(_format.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertices.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indices.get());
		LUCID_GAL_PIPELINE.drawInstanced(_topology, _vertices->count(), _indices->count(), int(count));
	LUCID_GAL_PIPELINE.endGeometry(_format.get());
}

void FixedGeometry::initialize(LUCID_CORE::Reader &reader)
{
	reader.read(&_topology, sizeof(LUCID_GAL::Pipeline::TOPOLOGY));

    if (reader.read<bool>())
        _format.reset(LUCID_GAL::VertexFormat::create(reader.read<std::string>()));
    else
    	_format.reset(LUCID_GAL::VertexFormat::create(reader));

	_vertices.reset(LUCID_GAL::VertexBuffer::create(reader));
	_indices.reset(LUCID_GAL::IndexBuffer::create(reader));		
}

void FixedGeometry::shutdown()
{
	///	NOP for now
}

///
///	AdaptiveGeometry
///

AdaptiveGeometry::AdaptiveGeometry(LUCID_CORE::Reader &reader)
{
    initialize(reader);
}

AdaptiveGeometry::~AdaptiveGeometry()
{
    shutdown();
}

void AdaptiveGeometry::initialize(LUCID_CORE::Reader &reader)
{
    shutdown();

    _vertexMaximum = reader.read<uint32_t>();
    _faceMaximum = reader.read<uint32_t>();
    uint32_t depthInitial = reader.read<uint32_t>();

    uint32_t vertexCount = reader.read<uint32_t>();
    _vertices.resize(vertexCount);
    reader.read(&_vertices[0], vertexCount * sizeof(Vertex));

    uint32_t faceCount = reader.read<uint32_t>();
    _faces.resize(faceCount);
    reader.read(&_faces[0], faceCount * sizeof(Face));

    _adjacency.clear();
    for (uint32_t i = 0; i < faceCount; ++i)
    {
        Face const &face = _faces[i];

        self_createAdjacency(face. base(), i);
        self_createAdjacency(face.right(), i);
        self_createAdjacency(face. left(), i);
    }

    uint32_t first = 0;
    for (uint32_t depth = 0; depth < depthInitial; ++depth)
    {
        uint32_t end = uint32_t(_faces.size());
        for (uint32_t i = first; i < end; ++i)
            splitFace(i);
        first = end;
    }

    /// test {
    _heightmap.initialize(LUCID_CORE::FileReader("content/_399/earth.heightmap"), 8, 8);

    first = 0;
    for (uint32_t depth = 0; depth < 5; ++depth)
    {
        uint32_t end = uint32_t(_faces.size());
        for (uint32_t i = first; i < end; ++i)
        {
            Face face = _faces[i];
        
            /// non-const copies
            /// if one, or two, texcoords are on the seam
            /// they will have to be adjusted
            LUCID_GAL::Vector2 p = _vertices[face[0]].texcoord;
            LUCID_GAL::Vector2 q = _vertices[face[1]].texcoord;
            LUCID_GAL::Vector2 r = _vertices[face[2]].texcoord;

            bool right = (p.x > 0.5f) || (q.x > 0.5f) || (r.x > 0.5f);

            p.x = ((0 == p.x) && right) ? 1.f : p.x;
            q.x = ((0 == q.x) && right) ? 1.f : q.x;
            r.x = ((0 == r.x) && right) ? 1.f : r.x;

            Heightmap::Tile const &tile = _heightmap.filter(p, q, r);
            if ((tile.h[0] < 128) && (128 < tile.h[1]))
                splitFace(i);
        }
        first = end;
    }
    /// } test

    _vertexFormat.reset(LUCID_GAL::VertexFormat::create("content/sphere.format"));
    _vertexBuffer.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, _vertexMaximum, sizeof(Vertex)));
    _indexBuffer .reset(LUCID_GAL:: IndexBuffer::create(LUCID_GAL::IndexBuffer::USAGE_DYNAMIC, LUCID_GAL::IndexBuffer::FORMAT_UINT32, 3 * _faceMaximum));

    pushChanges();
}

void AdaptiveGeometry::shutdown()
{
	///	NOP for now
}

void AdaptiveGeometry::draw() const
{
    LUCID_GAL_PIPELINE.beginGeometry(_vertexFormat.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertexBuffer.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indexBuffer.get());
		LUCID_GAL_PIPELINE.draw(_topology, _vertexCount, _indexCount);
	LUCID_GAL_PIPELINE.endGeometry(_vertexFormat.get());
}

void AdaptiveGeometry::drawInstanced(int32_t count) const
{
	LUCID_GAL_PIPELINE.beginGeometry(_vertexFormat.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertexBuffer.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indexBuffer.get());
		LUCID_GAL_PIPELINE.drawInstanced(_topology, _vertexCount, _indexCount, count);
	LUCID_GAL_PIPELINE.endGeometry(_vertexFormat.get());
}

void AdaptiveGeometry::splitFace(uint32_t index)
{
    // make copy, not reference, altering collection
    Face const face = _faces[index];

    if (face.notLeaf())
        return;

    uint32_t neighborIndex = _adjacency[face.base()];
    Face neighbor = _faces[neighborIndex];

    if (notDiamond(face, neighbor))
        splitFace(neighborIndex);

    splitDiamond(index, _adjacency[face.base()]);
}

void AdaptiveGeometry::pushChanges()
{
    /// test {
    size_t faceCount = _faces.size();
    _indexCount = 0;

    if (0 == faceCount)
        return;

    uint32_t *indices = _indexBuffer->lock_as<uint32_t>();
    for (size_t faceIndex = 0; faceIndex < faceCount; ++faceIndex)
    {
        Face const &face = _faces[faceIndex];
        if (face.notLeaf())
            continue;

        uint32_t i = face.vertex[0];
        uint32_t j = face.vertex[1];
        uint32_t k = face.vertex[2];

        Vertex const &p = _vertices[i];
        Vertex const &q = _vertices[j];
        Vertex const &r = _vertices[k];

        uint16_t h[3] = {
            _heightmap(p.texcoord),
            _heightmap(q.texcoord),
            _heightmap(r.texcoord),
        };

        uint8_t code = ((0 != h[2]) << 2) | ((0 != h[1]) << 1) | ((0 != h[0]) << 0);

        if ((0 == code) || (7 == code))
        {
            indices[_indexCount + 0] = i;
            indices[_indexCount + 1] = j;
            indices[_indexCount + 2] = k;

            _indexCount = _indexCount + 3;
        }
        else
        {
            uint32_t l = makeVertex(p.position, q.position);
            uint32_t m = makeVertex(q.position, r.position);
            uint32_t n = makeVertex(r.position, p.position);

            indices[_indexCount +  0] = n;
            indices[_indexCount +  1] = m;
            indices[_indexCount +  2] = k;

            indices[_indexCount +  3] = i;
            indices[_indexCount +  4] = l;
            indices[_indexCount +  5] = n;

            indices[_indexCount +  6] = l;
            indices[_indexCount +  7] = m;
            indices[_indexCount +  8] = n;

            indices[_indexCount +  9] = l;
            indices[_indexCount + 10] = j;
            indices[_indexCount + 11] = m;

            _indexCount = _indexCount + 12;

            switch(code)
            {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            }
        }
    }
    _indexBuffer->unlock();

    _vertexCount = uint32_t(_vertices.size());
    ::memcpy(_vertexBuffer->lock(), &_vertices[0], _vertexCount * sizeof(Vertex));
    _vertexBuffer->unlock();
    /// } test
}

void AdaptiveGeometry::splitDiamond(uint32_t lhsIndex, uint32_t rhsIndex)
{
    // make copies, not references, altering collection
    Face const lhs = _faces[lhsIndex];
    Face const rhs = _faces[rhsIndex];

    // diamond condition is required before calling this method.
    assert(isDiamond(lhs, rhs) && "attempt to split non-diamond configuration");

    uint32_t i = lhs.vertex[0];
    uint32_t j = lhs.vertex[1];
    uint32_t k = lhs.vertex[2];
    uint32_t l = rhs.vertex[2];

    uint32_t m = makeVertex(_vertices[i].position, _vertices[j].position);

    Face const A = Face(lhsIndex, k, i, m);
    Face const B = Face(rhsIndex, i, l, m);
    Face const C = Face(rhsIndex, l, j, m);
    Face const D = Face(lhsIndex, j, k, m);

    uint32_t a = addFace(A);
    uint32_t b = addFace(B);
    uint32_t c = addFace(C);
    uint32_t d = addFace(D);

    self_updateAdjacency(A. base(), a);
    self_createAdjacency(A.right(), a);
    self_createAdjacency(A. left(), a);

    self_updateAdjacency(B. base(), b);
    self_createAdjacency(B.right(), b);
    self_createAdjacency(B. left(), b);

    self_updateAdjacency(C. base(), c);
    self_createAdjacency(C.right(), c);
    self_createAdjacency(C. left(), c);

    self_updateAdjacency(D. base(), d);
    self_createAdjacency(D.right(), d);
    self_createAdjacency(D. left(), d);

    // TBD: don't know if this is required/desired.
    // after spitting these parent faces, the adjacency does not
    // interfere with anything.  Also, it might be desirable to
    // keep this data intact for when the children are collapsed.
#if false
    removeAdjacency(lhs.base());
    removeAdjacency(rhs.base());
#endif

    // now, update the contained/actual faces that were split.

    _faces[lhsIndex].child[0] = d;
    _faces[lhsIndex].child[1] = a;

    _faces[rhsIndex].child[0] = b;
    _faces[rhsIndex].child[1] = c;
}

LUCID_GIGL_END