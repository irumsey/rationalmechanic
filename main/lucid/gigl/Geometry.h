#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <lucid/core/Identity.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Numbers.h>
#include <lucid/math/Math.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Types.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_BEGIN

class VertexFormat;
class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

///	Geometry
/// 
///	Note: acts as its own factory via Geometry::create(...)
class Geometry abstract
{
public:
	virtual ~Geometry() = default;

	virtual LUCID_CORE::Identity const &identity() const = 0;

	virtual LUCID_GAL::Pipeline::TOPOLOGY topology() const = 0;

	virtual uint32_t vertexCount() const = 0;

	virtual uint32_t indexCount() const = 0;

	virtual uint32_t primitiveCount() const = 0;

	virtual void draw() const = 0;

	/// Note: assumes per-instance stream has been set
	virtual void drawInstanced(int32_t count) const = 0;

	static Geometry *create(std::string const &path);

	static Geometry *create(LUCID_CORE::Reader &reader);

public:
	Geometry() = default;

private:

	LUCID_PREVENT_COPY(Geometry);
	LUCID_PREVENT_ASSIGNMENT(Geometry);
};

///	FixedGeometry
///
///
class FixedGeometry final : public Geometry
{
public:
	FixedGeometry(LUCID_CORE::Reader &reader);

	virtual ~FixedGeometry();

	virtual LUCID_CORE::Identity const &identity() const override;

	virtual LUCID_GAL::Pipeline::TOPOLOGY topology() const override;

	virtual uint32_t vertexCount() const override;

	virtual uint32_t indexCount() const override;

	virtual uint32_t primitiveCount() const override;

	virtual void draw() const override;

	/// Note: assumes per-instance stream has been set
	virtual void drawInstanced(int32_t count) const override;

private:
	LUCID_CORE::Identity const _identity;

	LUCID_GAL::Pipeline::TOPOLOGY _topology = LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_LIST;

	std::unique_ptr<LUCID_GAL::VertexFormat> _format;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _vertices;
	std::unique_ptr<LUCID_GAL::IndexBuffer> _indices;

	void initialize(LUCID_CORE::Reader &reader);

	void shutdown();

	LUCID_PREVENT_COPY(FixedGeometry);
	LUCID_PREVENT_ASSIGNMENT(FixedGeometry);
};

inline LUCID_CORE::Identity const &FixedGeometry::identity() const
{
	return _identity;
}

inline LUCID_GAL::Pipeline::TOPOLOGY FixedGeometry::topology() const
{
	return _topology;
}

inline uint32_t FixedGeometry::primitiveCount() const
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

/// AdaptiveGeometry
///
/// Note: in development.  there are two land mines in the form of
/// vertex and face maximums.  if exceeded, this just throws an error.
/// in the future, the refinement method should detect the condition
/// and stop.
/// 
/// Note: the refinement method is deferred.  methods are exposed which
/// allow for refinement, then the user pushes the changes.
/// 
/// Note: split/collapse face methods are used, obviously, for refining the geometry.
/// however, should be used sparingly and the surface can change by traversing
/// the heirarchy and picking the appropriate faces.
/// 
/// Note: for now, this assumes a spherical initial geometry.  or at least,
/// all face sides are connected.  would be "easy" to generalize, however, just
/// need spheres right now.
class AdaptiveGeometry final : public Geometry
{
public:
    /// Vertex
    ///
    /// 
    struct Vertex
    {
        LUCID_GAL::Vector3 position;
        LUCID_GAL::Vector3 tangent;
        LUCID_GAL::Vector2 texcoord;
    };

    AdaptiveGeometry(LUCID_CORE::Reader &reader);

    virtual ~AdaptiveGeometry();

	virtual LUCID_CORE::Identity const &identity() const override;

	/// Note: this will always return triangle list
	virtual LUCID_GAL::Pipeline::TOPOLOGY topology() const override;

	virtual uint32_t vertexCount() const override;

	virtual uint32_t indexCount() const override;

	/// Note: primitives will always be a face (triangle)
	virtual uint32_t primitiveCount() const override;

	virtual void draw() const override;

	virtual void drawInstanced(int32_t count) const override;

    Vertex const &getVertex(uint32_t index) const;

    Face const &getFace(uint32_t index) const;

    void splitFace(uint32_t index);

	void collapseFace(uint32_t index);

	void beginRefinement();

	///	used to finalize editing after split/collapse calls
	void endRefinement();

	void updateBuffers(std::vector<uint32_t> const &surface);

private:
    LUCID_CORE::Identity const _identity;

	LUCID_GAL::Pipeline::TOPOLOGY const _topology = LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_LIST;

	uint32_t _vertexMaximum = 0;
	uint32_t _faceMaximum = 0;

    std::vector<Vertex> _vertices;
    std::vector<Face> _faces;
    Adjacency _adjacency;

	std::vector<uint32_t> _removedVertices;
    std::vector<uint32_t> _vertexMapping;

	std::vector<uint32_t> _removedFaces;
	std::vector<uint32_t> _faceMapping;

	/// rendering counts...
	uint32_t _vertexCount = 0;
	uint32_t _indexCount = 0;

	std::unique_ptr<LUCID_GAL::VertexFormat> _vertexFormat;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _vertexBuffer;
	std::unique_ptr<LUCID_GAL::IndexBuffer> _indexBuffer;

    void initialize(LUCID_CORE::Reader &reader);

    void shutdown();

    LUCID_GAL::Vector2 computeTexcoord(LUCID_GAL::Vector3 const &position) const;

    bool isDiamond(Face const &lhs, Face const &rhs) const;

    bool notDiamond(Face const &lhs, Face const &rhs) const;

    uint32_t addVertex(Vertex const &vertex);

	uint32_t makeVertex(LUCID_GAL::Vector3 const &p, LUCID_GAL::Vector3 const &q);

    uint32_t addFace(Face const &face);

	uint32_t faceFromEdge(Face &lval, Edge const &edge) const;

    void createAdjacency(Edge const &edge, uint32_t index);

    void self_createAdjacency(Edge const &edge, uint32_t index);

    void removeAdjacency(Edge const &edge);

    void updateAdjacency(Edge const &edge, uint32_t index);

    void self_updateAdjacency(Edge const &edge, uint32_t index);

    void splitDiamond(uint32_t lhsIndex, uint32_t rhsIndex);

    LUCID_PREVENT_COPY(AdaptiveGeometry);
    LUCID_PREVENT_ASSIGNMENT(AdaptiveGeometry);
};

inline LUCID_CORE::Identity const &AdaptiveGeometry::identity() const
{
	return _identity;
}

inline LUCID_GAL::Pipeline::TOPOLOGY AdaptiveGeometry::topology() const
{
	return _topology;
}

inline uint32_t AdaptiveGeometry::vertexCount() const
{
    size_t count = _vertices.size();
    LUCID_VALIDATE(count < _vertexMaximum, "vertex maximum exceeded");

    return uint32_t(count);
}

inline uint32_t AdaptiveGeometry::indexCount() const
{
	return 3 * primitiveCount();
}

inline AdaptiveGeometry::Vertex const &AdaptiveGeometry::getVertex(uint32_t index) const
{
    assert((index < _vertices.size()) && "index out of bounds");
    return _vertices[index];
}

inline uint32_t AdaptiveGeometry::primitiveCount() const
{
	size_t count = _faces.size();
	LUCID_VALIDATE(count < _faceMaximum, "primitive count exceeded");

    return uint32_t(count);
}

inline Face const &AdaptiveGeometry::getFace(uint32_t index) const
{
	size_t count = _faces.size();
	LUCID_VALIDATE(count < _faceMaximum, "primitive count exceeded");

    assert((index < count) && "index out of bounds");
    return _faces[index];
}

inline LUCID_GAL::Vector2 AdaptiveGeometry::computeTexcoord(LUCID_GAL::Vector3 const &position) const
{
    LUCID_GAL::Vector3 normal = LUCID_MATH::normalize(position);
    LUCID_GAL::Scalar   theta = LUCID_MATH::acos(normal.z);
    LUCID_GAL::Scalar     phi = LUCID_MATH::atan2(normal.y, normal.x);

    phi = (phi < 0.f) ? (phi + LUCID_CORE_NUMBERS::two_pi<LUCID_GAL::Scalar>) : phi;

    // this will have a seam where u jumps from 1 to 0
    return LUCID_GAL::Vector2(
        LUCID_CORE_NUMBERS::inv_two_pi<LUCID_GAL::Scalar> * phi,
        LUCID_CORE_NUMBERS::inv_pi<LUCID_GAL::Scalar> * theta
    );
}

inline bool AdaptiveGeometry::isDiamond(Face const &lhs, Face const &rhs) const
{
    return lhs.base() == Edge::reverse(rhs.base());
}

inline bool AdaptiveGeometry::notDiamond(Face const &lhs, Face const &rhs) const
{
    return !isDiamond(lhs, rhs);
}

inline uint32_t AdaptiveGeometry::addVertex(Vertex const &vertex)
{
    size_t index = _vertices.size();
    LUCID_VALIDATE(index < _vertexMaximum,  "vertex maximum exceeded");

    _vertices.push_back(vertex);
    return uint32_t(index);
}

inline uint32_t AdaptiveGeometry::makeVertex(LUCID_GAL::Vector3 const &p, LUCID_GAL::Vector3 const &q)
{
    Vertex vertex;
    vertex.position = LUCID_MATH::normalize(0.5f * (p + q));
    vertex.texcoord = computeTexcoord(vertex.position);
    return addVertex(vertex);
}

inline uint32_t AdaptiveGeometry::addFace(Face const &face)
{
    size_t index = _faces.size();
    _faces.push_back(face);
    return uint32_t(index);
}

inline uint32_t AdaptiveGeometry::faceFromEdge(Face &lval, Edge const &rhs) const
{
    auto iter = _adjacency.find(rhs);
    LUCID_VALIDATE(iter == _adjacency.end(), "face not found using supplied edge");

    uint32_t index = iter->second;
    lval = getFace(index);

    return index; 
}

inline void AdaptiveGeometry::createAdjacency(Edge const &edge, uint32_t index)
{
    assert((_adjacency.end() == _adjacency.find(edge)) && "adjacency already exists");
    _adjacency.insert(std::make_pair(edge, index));
}

inline void AdaptiveGeometry::self_createAdjacency(Edge const &edge, uint32_t index)
{
    createAdjacency(Edge::reverse(edge), index);
}

inline void AdaptiveGeometry::removeAdjacency(Edge const &edge)
{
    _adjacency.erase(edge);
}

inline void AdaptiveGeometry::updateAdjacency(Edge const &edge, uint32_t index)
{
    auto iter = _adjacency.find(edge);
    assert((iter != _adjacency.end()) && "adjacency not defined");
    iter->second = index;
}

inline void AdaptiveGeometry::self_updateAdjacency(Edge const &edge, uint32_t index)
{
    updateAdjacency(Edge::reverse(edge), index);
}

LUCID_GIGL_END
