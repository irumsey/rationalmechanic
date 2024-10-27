#include "Geometry.h"
#include "Resources.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/FileReader.h>

///
///
///

LUCID_ANONYMOUS_BEGIN

template<typename T, typename I> inline bool removeElements(
    std::vector<T> &elements,
    std::vector<I> & mapping,
    std::vector<I> & removed
)
{
    if (removed.empty())
        return false;

    mapping.resize(elements.size());
    std::generate(mapping.begin(), mapping.end(), [i = 0]() mutable { return i++; });

    std::sort(removed.rbegin(), removed.rend());

    I last = I(elements.size() - 1);
    for (I index : removed)
    {
        assert(index < last);
        
        std::swap(elements[index], elements[last]);
        mapping[index] =  last;
        mapping[ last] = index;

        --last;
    }

    elements.resize(last + 1);

    return true;
}

LUCID_ANONYMOUS_END

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

    beginRefinement();
        uint32_t first = 0;
        for (uint32_t depth = 0; depth < depthInitial; ++depth)
        {
            uint32_t end = uint32_t(_faces.size());
            for (uint32_t i = first; i < end; ++i)
                splitFace(i);
            first = end;
        }    
    endRefinement();

    _vertexFormat.reset(LUCID_GAL::VertexFormat::create("content/sphere.format"));
    _vertexBuffer.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, _vertexMaximum, sizeof(Vertex)));
    _indexBuffer .reset(LUCID_GAL:: IndexBuffer::create(LUCID_GAL::IndexBuffer::USAGE_DYNAMIC, LUCID_GAL::IndexBuffer::FORMAT_UINT32, 3 * _faceMaximum));

    /// test {
    /// for testing, all leaf nodes makeup the surface
    std::vector<uint32_t> surface;
    for (uint32_t faceIndex = 0; faceIndex < _faces.size(); ++faceIndex)
    {
        Face const &face = _faces[faceIndex];
        if (face.notLeaf())
            continue;
        surface.push_back(faceIndex);
    }
    updateBuffers(surface);
    /// } test
}

void AdaptiveGeometry::shutdown()
{
	_vertexMaximum = 0;
	_faceMaximum = 0;
	_vertexCount = 0;
	_indexCount = 0;

    _vertices.clear();
    _faces.clear();
    _adjacency.clear();
    
    _removedVertices.clear();
	_removedFaces.clear();

	_vertexFormat.reset();
	_vertexBuffer.reset();
	_indexBuffer.reset();
}

void AdaptiveGeometry::draw() const
{
    if (0 == _vertexCount)
        return;

    LUCID_GAL_PIPELINE.beginGeometry(_vertexFormat.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertexBuffer.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indexBuffer.get());
		LUCID_GAL_PIPELINE.draw(_topology, _vertexCount, _indexCount);
	LUCID_GAL_PIPELINE.endGeometry(_vertexFormat.get());
}

void AdaptiveGeometry::drawInstanced(int32_t count) const
{
    if (0 == _vertexCount)
        return;

	LUCID_GAL_PIPELINE.beginGeometry(_vertexFormat.get());
		LUCID_GAL_PIPELINE.setVertexStream(0, _vertexBuffer.get());
		LUCID_GAL_PIPELINE.setIndexStream(_indexBuffer.get());
		LUCID_GAL_PIPELINE.drawInstanced(_topology, _vertexCount, _indexCount, count);
	LUCID_GAL_PIPELINE.endGeometry(_vertexFormat.get());
}

/// recursive until the supplied face is part of a diamond.
/// so far, only "spherical" geometry has been tested.  "spherical"
/// in that all faces have a neighbor.  this might mean the splitting
/// operation wraps around causing an error condition, however, this
/// has not happened and appears unlikely.
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

void AdaptiveGeometry::collapseFace(uint32_t index)
{
    // this method will not alter the collection of vertices
    // or faces, therefore we can take references.
    // this allows the user to collapse multiple faces, then
    // call pushChanges(...) for a one time update.
    Face const &A = getFace(index);

    if (A.notLeaf())
        return;

    if (A.isRoot())
        return;

    Face B;
    uint32_t b = faceFromEdge(B, A.right());

    Face C;
    uint32_t c = faceFromEdge(C, B.right());

    Face D;
    uint32_t d = faceFromEdge(D, C.right());

    // a and c do not share an edge therefore they are guaranteed
    // to have different parents (same for b and d).
    assert((A.parent != C.parent) && "consistency error");
    assert((B.parent != D.parent) && "consistency error");

    // all faces should share the same apex vertex
    assert((A.vertex[2] == B.vertex[2]) && "consistency error");
    assert((B.vertex[2] == C.vertex[2]) && "consistency error");

    Face &Pa = _faces[A.parent];
    Face &Pc = _faces[C.parent];

    assert((Pa.base() == Edge::reverse(Pc.base())) && "consistency errpr");

    self_updateAdjacency(Pa.right(), A.parent);
    self_updateAdjacency(Pa. left(), A.parent);

    self_updateAdjacency(Pc.right(), C.parent);
    self_updateAdjacency(Pc. left(), C.parent);

    _removedVertices.push_back(A.vertex[2]);

    _removedFaces.push_back(index);
    _removedFaces.push_back(b);
    _removedFaces.push_back(c);
    _removedFaces.push_back(d);

    Pa.child[0] = Pa.child[1] = Face::INVALID_INDEX;
    Pc.child[0] = Pc.child[1] = Face::INVALID_INDEX;
}

void AdaptiveGeometry::beginRefinement()
{

}

void AdaptiveGeometry::endRefinement()
{
    bool verticesRemoved = removeElements(_vertices, _vertexMapping, _removedVertices);
    _removedVertices.clear();

    bool facesRemoved = removeElements(_faces, _faceMapping, _removedFaces);
    _removedFaces.clear();

    if (!(verticesRemoved || facesRemoved))
        return;

    for (Face &face : _faces)
    {
        if (facesRemoved)
        {
            face.  parent = face.isRoot() ? Face::INVALID_INDEX : _faceMapping[face.  parent];
            face.child[0] = face.isLeaf() ? Face::INVALID_INDEX : _faceMapping[face.child[0]];
            face.child[1] = face.isLeaf() ? Face::INVALID_INDEX : _faceMapping[face.child[1]];
        }

        if (verticesRemoved)
        {
            face.vertex[0] = _vertexMapping[face.vertex[0]];
            face.vertex[1] = _vertexMapping[face.vertex[1]];
            face.vertex[2] = _vertexMapping[face.vertex[2]];
        }
    }

    _vertexMapping.clear();
    _faceMapping.clear();
}

void AdaptiveGeometry::updateBuffers(std::vector<uint32_t> const &surface)
{
    _vertexCount = uint32_t(_vertices.size());
    std::memcpy(_vertexBuffer->lock(), &_vertices[0], _vertexCount * sizeof(Vertex));
    _vertexBuffer->unlock();

    uint32_t *indices = _indexBuffer->lock_as<uint32_t>();
    _indexCount = 0;

    for (uint32_t faceIndex : surface)
    {
        Face const &face = _faces[faceIndex];

        indices[_indexCount + 0] = face.vertex[0];
        indices[_indexCount + 1] = face.vertex[1];
        indices[_indexCount + 2] = face.vertex[2];

        _indexCount += 3;
    }

    _indexBuffer->unlock();
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

    // update the contained/actual faces that were split.

    _faces[lhsIndex].child[0] = d;
    _faces[lhsIndex].child[1] = a;

    _faces[rhsIndex].child[0] = b;
    _faces[rhsIndex].child[1] = c;
}

LUCID_GIGL_END