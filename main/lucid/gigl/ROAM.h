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
            (vertex[1] != INVALID_INDEX<Ti>)
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

/// Geometry
///
/// 
template<typename Ti> struct Geometry
{
    typedef Edge<Ti> edge_t;
    typedef Face<Ti> face_t;

    std::vector<face_t> faces;
    std::vector<bool> surface;
    Adjacency<Ti> adjacency;

    Geometry() = default;

    ~Geometry() = default;

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
        assert((index < surface.size()) && "index out-of-bounds");
        return surface[index];
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

    LUCID_PREVENT_COPY(Geometry);
    LUCID_PREVENT_ASSIGNMENT(Geometry);
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
template<typename Ti> inline void create_adjacency(Geometry<Ti> &geometry, Edge<Ti> const &edge, size_t index)
{
    assert((geometry.adjacency.end() == geometry.adjacency.find(edge)) && "adjacency already exists");
    geometry.adjacency[edge] = index;
}

/// update_adjacency
/// 
/// 
template<typename Ti> inline void update_adjacency(Geometry<Ti> &geometry, Edge<Ti> const &edge, size_t index)
{
    assert((geometry.adjacency.end() != geometry.adjacency.find(edge)) && "adjacency not defined");
    geometry.adjacency[edge] = index;
}

/// 
/// 
/// 
template<typename Ti> inline size_t add_face(Geometry<Ti> &geometry, Face<Ti> const &face)
{
    assert((geometry.faces.size() == geometry.surface.size()) && "consistency error");

    size_t index = geometry.faces.size();

    geometry.faces.push_back(face);
    geometry.surface.push_back(true);

    update_adjacency(geometry, reverse(face. base()), index);
    create_adjacency(geometry, reverse(face.right()), index);
    create_adjacency(geometry, reverse(face. left()), index);

    return index;
}

/// split_isolated_face
///
/// 
template<typename Ti, typename Fn> inline void split_isolated_face(Geometry<Ti> &geometry, Fn &split_edge, size_t index)
{
    assert(geometry.is_leaf(index) && "attempt to split non-leaf face");

    Ti i = geometry[index][0];
    Ti j = geometry[index][1];
    Ti k = geometry[index][2];

    Ti l = split_edge(geometry[index].base());

    add_face(geometry, Face<Ti>(k, i, l));
    add_face(geometry, Face<Ti>(j, k, l));

    geometry.surface[index] = false;
}

/// split_diamond
///
/// 
template<typename Ti, typename Fn> inline void split_diamond(Geometry<Ti> &geometry, Fn &split_edge, size_t p, size_t q)
{
    assert(is_diamond(geometry[p], geometry[q]) && "attempt to split non-diamond configuration");
    assert((geometry.is_leaf(p) && geometry.is_leaf(q)) && "attempt to split non-leaf face");

    Ti i = geometry[p][0];
    Ti j = geometry[p][1];
    Ti k = geometry[p][2];
    Ti l = geometry[q][2];

    Ti m = split_edge(geometry[p].base());

    add_face(geometry, Face<Ti>(k, i, m));
    add_face(geometry, Face<Ti>(i, l, m));
    add_face(geometry, Face<Ti>(l, j, m));
    add_face(geometry, Face<Ti>(j, k, m));

    geometry.surface[p] = false;
    geometry.surface[q] = false;
}

/// split_face
///
/// Note: recursive.  if the base neighbor of the specified face does not
/// also have the specified face as its base neighbor, this method will force
/// a split of the neighbor.
template<typename Ti, typename Fn> inline void split_face(Geometry<Ti> &geometry, Fn &split_edge, size_t index)
{
    if (geometry.not_leaf(index))
        return;

    size_t baseIndex = geometry.neighbor(geometry[index].base());
    if (INVALID_INDEX<Ti> == baseIndex)
    {
        split_isolated_face(geometry, split_edge, index);
    }
    else
    {
        if (not_diamond(geometry[index], geometry[baseIndex]))
            split_face(geometry, split_edge, baseIndex);
        split_diamond(geometry, split_edge, index, geometry.neighbor(geometry[index].base()));
    }
}

/// initialize
///
/// after filling up the geometry's face container, calling this
/// will setup leaf flag and adjacency data.
template<typename Ti> inline void initialize(Geometry<Ti> &geometry)
{
    size_t count = geometry.faces.size();
    geometry.surface.resize(count, true);

    geometry.adjacency.clear();
    for (size_t i = 0; i < count; ++i)
    {
        Face<Ti> const &face = geometry[i];

        create_adjacency(geometry, reverse(face. base()), i);
        create_adjacency(geometry, reverse(face.right()), i);
        create_adjacency(geometry, reverse(face. left()), i);
    }
}

/// strip_non_leaves
///
/// helper for creating an initial tesselation, or starting point, for
/// future refinement.  this preserves all leaf faces then resets
/// the geometry with the remaining faces.
template<typename Ti> inline void strip_non_leaves(Geometry<Ti> &geometry)
{
    assert((geometry.faces.size() == geometry.surface.size()) && "consistency error");

    auto face = geometry. faces.begin();
    auto leaf = geometry.surface.begin();

    while (face != geometry.faces.end())
    {
        if (*leaf)
        {
            ++face;
            ++leaf;
        }
        else
        {
            face = geometry. faces.erase(face);
            leaf = geometry.surface.erase(leaf);
        }
    }

    initialize(geometry);
}

ROAM_END
