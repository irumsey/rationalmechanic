#pragma once

#include <lucid/math/Math.h>
#include <lucid/core/Numbers.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Types.h>

LUCID_GIGL_BEGIN

/// Sphere
///
/// 

class Sphere
{
public:
    static size_t const VERTEX_MAXIMUM = (1 << 16);

    /// Vertex
    ///
    /// 
    struct Vertex
    {
        LUCID_GAL::Vector3 position;
        LUCID_GAL::Vector2 texcoord;
    };

    Sphere(size_t depthInitial = 1);

    virtual ~Sphere() = default;

    void initialize(size_t depthInitial);

    uint16_t vertexCount() const;

    Vertex const &vertex(uint16_t index) const;

    size_t faceCount() const;

    Face const &face(size_t index) const;

    /// TBD: a more generic refine method...
    /// void refine(Field const &field);

private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    Adjacency adjacency;

    LUCID_GAL::Vector2 computeTexcoord(LUCID_GAL::Vector3 const &position) const;

    bool isDiamond(Face const &lhs, Face const &rhs) const;

    bool notDiamond(Face const &lhs, Face const &rhs) const;

    uint16_t addVertex(Vertex const &vertex);

    size_t addFace(Face const &face);

    void createAdjacency(Edge const &edge, size_t index);

    void self_createAdjacency(Edge const &edge, size_t index);

    void removeAdjacency(Edge const &edge);

    void updateAdjacency(Edge const &edge, size_t index);

    void self_updateAdjacency(Edge const &edge, size_t index);

    void splitFace(size_t index);

    void splitDiamond(size_t lhsIndex, size_t rhsIndex);

};

inline uint16_t Sphere::vertexCount() const
{
    size_t count = vertices.size();
    assert((count < VERTEX_MAXIMUM) && "vertex maximum exceeded");

    return uint16_t(count);
}

inline Sphere::Vertex const &Sphere::vertex(uint16_t index) const
{
    assert((index < vertices.size()) && "index out of bounds");
    return vertices[index];
}

inline size_t Sphere::faceCount() const
{
    return faces.size();
}

inline Face const &Sphere::face(size_t index) const
{
    return faces[index];
}

inline LUCID_GAL::Vector2 Sphere::computeTexcoord(LUCID_GAL::Vector3 const &position) const
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

inline bool Sphere::isDiamond(Face const &lhs, Face const &rhs) const
{
    return lhs.base() == Edge::reverse(rhs.base());
}

inline bool Sphere::notDiamond(Face const &lhs, Face const &rhs) const
{
    return !isDiamond(lhs, rhs);
}

inline uint16_t Sphere::addVertex(Vertex const &vertex)
{
    size_t index = vertices.size();
    assert((index < (VERTEX_MAXIMUM - 1)) && "vertex maximum exceeded");

    vertices.push_back(vertex);
    return uint16_t(index);
}

inline size_t Sphere::addFace(Face const &face)
{
    size_t index = faces.size();
    faces.push_back(face);
    return index;
}

inline void Sphere::createAdjacency(Edge const &edge, size_t index)
{
    assert((adjacency.end() == adjacency.find(edge)) && "adjacency already exists");
    adjacency.insert(std::make_pair(edge, index));
}

inline void Sphere::self_createAdjacency(Edge const &edge, size_t index)
{
    createAdjacency(Edge::reverse(edge), index);
}

inline void Sphere::removeAdjacency(Edge const &edge)
{
    adjacency.erase(edge);
}

inline void Sphere::updateAdjacency(Edge const &edge, size_t index)
{
    auto iter = adjacency.find(edge);
    assert((iter != adjacency.end()) && "adjacency not defined");
    iter->second = index;
}

inline void Sphere::self_updateAdjacency(Edge const &edge, size_t index)
{
    updateAdjacency(Edge::reverse(edge), index);
}

LUCID_GIGL_END
