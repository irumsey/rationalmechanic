#pragma once

#include <cassert>
#include <unordered_map>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

///
/// The following types: Edge, Face, and Adjacency are intended for use with
/// adaptive style geometry.  However, their use need not be limited to said
/// gemoetry.  They are basic definitions of common concepts.
/// 

/// Edge
///
/// Note: the order in which the vertices are listed matters.
/// two faces which share the same edge will have the order reversed.
/// See : Face
struct Edge
{
    static uint16_t const INVALID_INDEX = uint16_t(-1);

    uint16_t vertex[2] = { INVALID_INDEX, INVALID_INDEX, };

    Edge() = default;

    Edge(uint16_t i, uint16_t j)
    {
        vertex[0] = i;
        vertex[1] = j;
    }

    ~Edge() = default;

    uint16_t const &operator[](size_t index) const
    {
        assert((index < 2) && "index out of bounds");
        return vertex[index];
    }

    uint16_t &operator[](size_t index)
    {
        assert((index < 2) && "index out of bounds");
        return vertex[index];
    }

    static Edge reverse(Edge const &rhs)
    {
        return Edge(rhs[1], rhs[0]);
    }

    static bool equal(Edge const &lhs, Edge const &rhs)
    {
        return (lhs.vertex[0] == rhs.vertex[0]) && (lhs.vertex[1] == rhs.vertex[1]);
    }

    struct Hash
    {
        size_t operator()(Edge const &edge) const
        {
            size_t const  mask = 0x0000ffff;
            size_t const shift = 16;

            return ((mask & size_t(edge[0])) << shift) | (mask & size_t(edge[1]));
        }
    };

    struct Equal
    {
        bool operator()(Edge const &lhs, Edge const &rhs) const
        {
            return Edge::equal(lhs, rhs);
        }
    };

};

/// Face
///
/// 
/// Rules:
///     index (0, 1) is base edge
///     index (1, 2) is right edge
///     index (2, 0) is left edge
/// Note : vertices are listed in strict CCW order, always.
struct Face
{
    static size_t const INVALID_FACE = size_t(-1);
    static uint16_t const INVALID_VERTEX = uint16_t(-1);

    size_t parent = INVALID_FACE;
    size_t children[2] = { INVALID_FACE, INVALID_FACE, };

    uint16_t vertex[3] = { INVALID_VERTEX, INVALID_VERTEX, INVALID_VERTEX, };
    
    Face() = default;

    Face(uint16_t i, uint16_t j, uint16_t k)
    {
        vertex[0] = i;
        vertex[1] = j;
        vertex[2] = k;
    }

    Face(size_t parent, uint16_t i, uint16_t j, uint16_t k)
        : parent(parent)
    {
        vertex[0] = i;
        vertex[1] = j;
        vertex[2] = k;
    }

    ~Face() = default;

    uint16_t const &operator[](size_t index) const
    {
        assert((index < 3) && "index out of bounds");
        return vertex[index];
    }

    uint16_t &operator[](size_t index)
    {
        assert((index < 3) && "index out of bounds");
        return vertex[index];
    }

    void setChildren(size_t i, size_t j)
    {
        children[0] = i;
        children[1] = j;
    }

    bool isLeaf() const
    {
        return (INVALID_FACE == children[0]) && (INVALID_FACE == children[1]);
    }

    bool notLeaf() const
    {
        return !isLeaf();
    }

    Edge base() const
    {
        return Edge(vertex[0], vertex[1]);
    }

    Edge right() const
    {
        return Edge(vertex[1], vertex[2]);
    }

    Edge left() const
    {
        return Edge(vertex[2], vertex[0]);
    }

};

/// Adjacency
/// 
/// Note: a given face's edge "points-to" the neighbor which shares that edge.
/// Therefore, given a face, one can get the neighbors by looking up each edge.
typedef std::unordered_map<Edge, size_t, Edge::Hash, Edge::Equal> Adjacency;

LUCID_GIGL_END

inline bool operator==(LUCID_GIGL::Edge const &lhs, LUCID_GIGL::Edge const &rhs)
{
    return LUCID_GIGL::Edge::equal(lhs, rhs);
}
