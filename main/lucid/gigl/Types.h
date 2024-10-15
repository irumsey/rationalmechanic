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
    static uint32_t const INVALID_INDEX = uint32_t(-1);

    uint32_t vertex[2] = { INVALID_INDEX, INVALID_INDEX, };

    Edge() = default;

    Edge(uint32_t i, uint32_t j)
    {
        vertex[0] = i;
        vertex[1] = j;
    }

    ~Edge() = default;

    uint32_t const &operator[](uint32_t index) const
    {
        assert((index < 2) && "index out of bounds");
        return vertex[index];
    }

    uint32_t &operator[](uint32_t index)
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
        uint32_t operator()(Edge const &edge) const
        {
            uint32_t const  mask = 0x0000ffff;
            uint32_t const shift = 16;

            return ((mask & edge[0]) << shift) | (mask & edge[1]);
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
    static uint32_t const INVALID_INDEX = uint32_t(-1);

    uint32_t parent    =   INVALID_INDEX;
    uint32_t child [2] = { INVALID_INDEX, INVALID_INDEX, };
    uint32_t vertex[3] = { INVALID_INDEX, INVALID_INDEX, INVALID_INDEX, };
    
    Face() = default;

    Face(uint32_t i, uint32_t j, uint32_t k)
    {
        vertex[0] = i;
        vertex[1] = j;
        vertex[2] = k;
    }

    Face(uint32_t parent, uint32_t i, uint32_t j, uint32_t k)
        : parent(parent)
    {
        vertex[0] = i;
        vertex[1] = j;
        vertex[2] = k;
    }

    ~Face() = default;

    uint32_t const &operator[](uint32_t index) const
    {
        validate();
        assert((index < 3) && "index out of bounds");
        return vertex[index];
    }

    uint32_t &operator[](uint32_t index)
    {
        validate();
        assert((index < 3) && "index out of bounds");
        return vertex[index];
    }

    bool isRoot() const
    {
        validate();
        return (INVALID_INDEX == parent);    
    }

    bool notRoot() const
    {
        validate();
        return !isRoot();
    }

    bool isLeaf() const
    {
        validate();
        return (INVALID_INDEX == child[0]) && (INVALID_INDEX == child[1]);
    }

    bool notLeaf() const
    {
        validate();
        return !isLeaf();
    }

    Edge base() const
    {
        validate();
        return Edge(vertex[0], vertex[1]);
    }

    Edge right() const
    {
        validate();
        return Edge(vertex[1], vertex[2]);
    }

    Edge left() const
    {
        validate();
        return Edge(vertex[2], vertex[0]);
    }

    void validate() const
    {
#if _DEBUG
        assert(
            ((INVALID_INDEX == child[0]) && (INVALID_INDEX == child[1])) ||
            ((INVALID_INDEX != child[0]) && (INVALID_INDEX != child[1]))
        );
#endif
    }
};

/// Adjacency
/// 
/// Note: a given face's edge "points-to" the neighbor which shares that edge.
/// Therefore, given a face, one can get the neighbors by looking them up by edge.
typedef std::unordered_map<Edge, uint32_t, Edge::Hash, Edge::Equal> Adjacency;

LUCID_GIGL_END

inline bool operator==(LUCID_GIGL::Edge const &lhs, LUCID_GIGL::Edge const &rhs)
{
    return LUCID_GIGL::Edge::equal(lhs, rhs);
}
