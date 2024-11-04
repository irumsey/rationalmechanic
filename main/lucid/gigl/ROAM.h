#pragma once

#include <cassert>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/Math.h>
#include <lucid/gigl/Defines.h>

#define ROAM_BEGIN LUCID_GIGL_BEGIN namespace roam {
#define ROAM_END   LUCID_GIGL_END }

ROAM_BEGIN

/// INVALID_INDEX
///
/// type used to validate an index and its type (16 or 32 bit).
/// Note: it uses the last index value as a sentinel.

template<typename T> struct invalid_index_type { static_assert("invalid index type"); };

template<typename T> constexpr T        INVALID_INDEX           = invalid_index_type<T>();
template<>           constexpr uint16_t INVALID_INDEX<uint16_t> =            uint16_t(-1);
template<>           constexpr uint32_t INVALID_INDEX<uint32_t> =            uint32_t(-1);

/// Edge
///
/// 
template<typename Ti> struct Edge
{
    Ti vertex[2] = { INVALID_INDEX<Ti>, INVALID_INDEX<Ti>, };

    Edge() = default;

    Edge(Ti i, Ti j)
    {
        vertex[0] = i;
        vertex[1] = j;
        validate();
    }

    ~Edge() = default;

    Ti const &operator[](size_t index) const
    {
        assert((index < 2) && "index out-of-bounds");
        validate();
        return vertex[index];
    }

    Ti &operator[](size_t index)
    {
        assert((index < 2) && "index out-of-bounds");
        validate();
        return vertex[index];
    }

    void validate() const
    {
#if defined(_DEBUG)
        assert(
            (vertex[0] != INVALID_INDEX<Ti>) &&
            (vertex[1] != INVALID_INDEX<Ti>) &&
            (vertex[2] != INVALID_INDEX<Ti>)
        );
#endif
    }

};

template<typename Ti> inline bool operator==(Edge<Ti> const &lhs, Edge<Ti> const &rhs)
{
    return (lhs[0] == rhs[0]) && (lhs[1] == rhs[1]);
}

template<typename Ti> inline Edge<Ti> reverse(Edge<Ti> const &rhs)
{
    return Edge<Ti>(rhs[1], rhs[0]);
}

template<typename Ti> struct edge_hash
{
    size_t operator()(Edge<Ti> const &edge) const
    {
        static_assert((8 == sizeof(size_t)) && "for now, assuming 8 bytes for size_t");

        size_t const  mask = 0x00000000ffffffff;
        size_t const shift = 32;

        return ((mask & size_t(edge[1])) << shift) | (mask & size_t(edge[0]));
    }
};

template<typename Ti> struct edge_equal
{
    bool operator()(Edge<Ti> const &lhs, Edge<Ti> const &rhs) const
    {
        return (lhs == rhs);
    }
};

/// Face
///
/// 
/// rules:
///     index (0, 1) is base edge
///     index (1, 2) is right edge
///     index (2, 0) is left edge
template<typename Ti> struct Face
{
    Ti vertex[3] = { INVALID_INDEX<Ti>, INVALID_INDEX<Ti>, INVALID_INDEX<Ti>, };
    
    Face() = default;

    Face(Ti i, Ti j, Ti k)
    {
        vertex[0] = i;
        vertex[1] = j;
        vertex[2] = k;
        validate();
    }

    ~Face() = default;

    Ti const &operator[](size_t index) const
    {
        assert((index < 3) && "index out-of-bounds");
        validate();
        return vertex[index];
    }

    Ti &operator[](size_t index)
    {
        assert((index < 3) && "index out-of-bounds");
        validate();
        return vertex[index];
    }

    Edge<Ti> base() const
    {
        validate();
        return Edge<Ti>(vertex[0], vertex[1]);
    }

    Edge<Ti> right() const
    {
        validate();
        return Edge<Ti>(vertex[1], vertex[2]);
    }

    Edge<Ti> left() const
    {
        validate();
        return Edge<Ti>(vertex[2], vertex[0]);
    }

    void validate() const
    {
#if defined(_DEBUG)
        assert(
            (vertex[0] != INVALID_INDEX<Ti>) &&
            (vertex[1] != INVALID_INDEX<Ti>) &&
            (vertex[2] != INVALID_INDEX<Ti>)
        );
#endif
    }
};

/// Adjacency
///
/// for a given edge, the associated value is the index of the
/// opposing face (see Face above.)  given a face, one can get
/// each of the three edges.  since all faces list their edges
/// and vertices in CCW order the following:
///     
///     size_t index = adjacency[face.right()];
/// 
/// will result in setting index equal to the index of face's right
/// edge neighbor.
template<typename Ti> using Adjacency = std::unordered_map<Edge<Ti>, size_t, edge_hash<Ti>, edge_equal<Ti> >;

/// Surface
///
/// 
template<typename Ti> struct Surface
{
    typedef Edge<Ti> edge_t;
    typedef Face<Ti> face_t;

    std::vector<face_t> faces;
    std::vector<bool> leaves;
    Adjacency<Ti> adjacency;

    Surface() = default;

    ~Surface() = default;

    Face<Ti> const &operator[](size_t index) const
    {
        assert((index < faces.size()) && "index out-of-bounds");
        return faces[index];
    }

    Face<Ti> &operator[](size_t index)
    {
        assert((index < faces.size()) && "index out-of-bounds");
        return faces[index];
    }

    bool is_leaf(size_t index) const
    {
        assert((index < leaves.size()) && "index out-of-bounds");
        return leaves[index];
    }

    bool not_leaf(size_t index) const
    {
        return !is_leaf(index);
    }

    size_t neighbor(Edge<Ti> const &edge) const
    {
        auto iter = adjacency.find(edge);
        return (adjacency.end() != iter) ? iter->second : INVALID_INDEX<Ti>;
    }

    LUCID_PREVENT_COPY(Surface);
    LUCID_PREVENT_ASSIGNMENT(Surface);
};

/// is_diamond and not_diamond
/// 
///

template<typename Ti> inline bool is_diamond(Face<Ti> const &A, Face<Ti> const &B)
{
    return A.base() == reverse(B.base());
}

template<typename Ti> inline bool not_diamond(Face<Ti> const &A, Face<Ti> const &B)
{
    return !is_diamond(A, B);
}

/// create_adjacency
/// 
/// 
template<typename Ti> inline void create_adjacency(Surface<Ti> &surface, Edge<Ti> const &edge, size_t index)
{
    assert((surface.adjacency.end() == surface.adjacency.find(edge)) && "adjacency already exists");
    surface.adjacency[edge] = index;
}

/// update_adjacency
/// 
/// 
template<typename Ti> inline void update_adjacency(Surface<Ti> &surface, Edge<Ti> const &edge, size_t index)
{
    assert((surface.adjacency.end() != surface.adjacency.find(edge)) && "adjacency not defined");
    surface.adjacency[edge] = index;
}

/// 
/// 
/// 
template<typename Ti> inline size_t add_face(Surface<Ti> &surface, Face<Ti> const &face)
{
    assert((surface.faces.size() == surface.leaves.size()) && "consistency error");

    size_t index = surface.faces.size();

    surface.faces.push_back(face);
    surface.leaves.push_back(true);

    update_adjacency(surface, reverse(face. base()), index);
    create_adjacency(surface, reverse(face.right()), index);
    create_adjacency(surface, reverse(face. left()), index);

    return index;
}

/// split_isolated_face
///
/// 
template<typename Ti, typename Fn> inline void split_isolated_face(Surface<Ti> &surface, Fn &make_vertex, size_t index)
{
    assert(surface.is_leaf(index) && "attempt to split non-leaf face");

    Ti i = surface[index][0];
    Ti j = surface[index][1];
    Ti k = surface[index][2];

    Ti l = make_vertex(index);

    add_face(surface, Face<Ti>(k, i, l));
    add_face(surface, Face<Ti>(j, k, l));

    surface.leaves[index] = false;
}

/// split_diamond
///
/// 
template<typename Ti, typename Fn> inline void split_diamond(Surface<Ti> &surface, Fn &make_vertex, size_t p, size_t q)
{
    assert(is_diamond(surface[p], surface[q]) && "attempt to split non-diamond configuration");
    assert((surface.is_leaf(p) && surface.is_leaf(q)) && "attempt to split non-leaf face");

    Ti i = surface[p][0];
    Ti j = surface[p][1];
    Ti k = surface[p][2];
    Ti l = surface[q][2];

    Ti m = make_vertex(p);

    add_face(surface, Face<Ti>(k, i, m));
    add_face(surface, Face<Ti>(i, l, m));
    add_face(surface, Face<Ti>(l, j, m));
    add_face(surface, Face<Ti>(j, k, m));

    surface.leaves[p] = false;
    surface.leaves[q] = false;
}

/// split_face
///
/// Note: recursive.  if the base neighbor of the specified face does not
/// also have the specified face as its base neighbor, this method will force
/// a split of the neighbor.
template<typename Ti, typename Fn> inline void split_face(Surface<Ti> &surface, Fn &make_vertex, size_t index)
{
    if (surface.not_leaf(index))
        return;

    size_t baseIndex = surface.neighbor(surface[index].base());
    if (INVALID_INDEX<Ti> == baseIndex)
    {
        split_isolated_face(surface, make_vertex, index);
    }
    else
    {
        if (not_diamond(surface[index], surface[baseIndex]))
            split_face(surface, make_vertex, baseIndex);
        split_diamond(surface, make_vertex, index, surface.neighbor(surface[index].base()));
    }
}

/// initialize
///
/// after filling up the surface's face container, calling this
/// will setup leaf flag and adjacency data.
template<typename Ti> inline void initialize(Surface<Ti> &surface)
{
    size_t count = surface.faces.size();
    surface.leaves.resize(count, true);

    surface.adjacency.clear();
    for (size_t i = 0; i < count; ++i)
    {
        Face<Ti> const &face = surface[i];

        create_adjacency(surface, reverse(face. base()), i);
        create_adjacency(surface, reverse(face.right()), i);
        create_adjacency(surface, reverse(face. left()), i);
    }
}

/// strip_non_leaves
///
/// helper for creating an initial tesselation, or starting point, for
/// future refinement.  this preserves all leaf faces then resets
/// the surface with the remaining faces.
template<typename Ti> inline void strip_non_leaves(Surface<Ti> &surface)
{
    assert((surface.faces.size() == surface.leaves.size()) && "consistency error");

    auto face = surface. faces.begin();
    auto leaf = surface.leaves.begin();

    while (face != surface.faces.end())
    {
        if (*leaf)
        {
            ++face;
            ++leaf;
        }
        else
        {
            face = surface. faces.erase(face);
            leaf = surface.leaves.erase(leaf);
        }
    }

    initialize(surface);
}

ROAM_END
