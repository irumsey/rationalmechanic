#include "Sphere.h"

LUCID_GIGL_BEGIN

Sphere::Sphere(size_t depthInitial)
{
    initialize(depthInitial);
}

void Sphere::initialize(size_t depthInitial)
{
    size_t const VERTEX_COUNT = 5;
    size_t const   FACE_COUNT = 6;

    vertices.resize(VERTEX_COUNT);
    faces.resize(FACE_COUNT);
    adjacency.clear();

    vertices[0] = {{ 0.000f,  0.000f,  1.f, }, {0, 0, }};
    vertices[1] = {{ 1.000f,  0.000f,  0.f, }, {0, 0, }};
    vertices[2] = {{-0.500f,  0.866f,  0.f, }, {0, 0, }};
    vertices[3] = {{-0.500f, -0.866f,  0.f, }, {0, 0, }};
    vertices[4] = {{ 0.000f,  0.000f, -1.f, }, {0, 0, }};

    for (size_t i = 0; i < VERTEX_COUNT; ++i)
    {
        vertices[i].position = LUCID_MATH::normalize(vertices[i].position);
        vertices[i].texcoord = computeTexcoord(vertices[i].position);
    }

    faces[0] = { 1, 2, 0, };
    faces[1] = { 2, 3, 0, };
    faces[2] = { 3, 1, 0, };
    faces[3] = { 1, 3, 4, };
    faces[4] = { 3, 2, 4, };
    faces[5] = { 2, 1, 4, };

    for (size_t i = 0; i < FACE_COUNT; ++i)
    {
        Face const &triangle = faces[i];

        self_createAdjacency(triangle. base(), i);
        self_createAdjacency(triangle.right(), i);
        self_createAdjacency(triangle. left(), i);
    }

    for (size_t depth = 0; depth < depthInitial; ++depth)
    {
        size_t count = faces.size();
        for (size_t i = 0; i < count; ++i)
            splitFace(i);
    }
}

#if false
void Sphere::refine(Field const &field)
{
    /// test {
    size_t first = 0;
    for (size_t pass = 0; pass < 8; ++pass)
    {
        size_t end = faces.size();
        for (size_t i = first; i < end; ++i)
        {
            Face face = faces[i];
        
            /// non-const copies
            /// if one, or two, texcoords are on the seam
            /// they will have to be adjusted
            vector2_t p = vertices[face[0]].texcoord;
            vector2_t q = vertices[face[1]].texcoord;
            vector2_t r = vertices[face[2]].texcoord;

            bool right = (p.x > 0.5f) || (q.x > 0.5f) || (r.x > 0.5f);

            p.x = ((0 == p.x) && right) ? 1.f : p.x;
            q.x = ((0 == q.x) && right) ? 1.f : q.x;
            r.x = ((0 == r.x) && right) ? 1.f : r.x;

            Field::Tile const &tile = field.filter(p, q, r);
            if (tile.h[1] != tile.h[0])
                splitFace(i);
        }
        first = end;
    }
    /// } test
}
#endif

void Sphere::splitFace(size_t index)
{
    Face const face = faces[index];
        
    if (face.notLeaf())
        return;

    size_t neighborIndex = adjacency[face.base()];
    Face neighbor = faces[neighborIndex];

    if (notDiamond(face, neighbor))
        splitFace(neighborIndex);

    splitDiamond(index, adjacency[face.base()]);
}

void Sphere::splitDiamond(size_t lhsIndex, size_t rhsIndex)
{
    // make copies, not references, this method will alter the collection.
    Face const lhs = faces[lhsIndex];
    Face const rhs = faces[rhsIndex];

    // diamond condition is required before calling this method.
    assert(isDiamond(lhs, rhs) && "attempt to split non-diamond configuration");

    uint16_t i = lhs.vertex[0];
    uint16_t j = lhs.vertex[1];
    uint16_t k = lhs.vertex[2];
    uint16_t l = rhs.vertex[2];

    Vertex vertex;
    vertex.position = LUCID_MATH::normalize(0.5f * (vertices[i].position + vertices[j].position));
    vertex.texcoord = computeTexcoord(vertex.position);
    uint16_t m = addVertex(vertex);

    Face A = Face(lhsIndex, k, i, m);
    Face B = Face(rhsIndex, i, l, m);
    Face C = Face(rhsIndex, l, j, m);
    Face D = Face(lhsIndex, j, k, m);

    size_t a = addFace(A);
    size_t b = addFace(B);
    size_t c = addFace(C);
    size_t d = addFace(D);

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

    removeAdjacency(lhs.base());
    removeAdjacency(rhs.base());

    // now, update the contained/actual faces that were split.
    faces[lhsIndex].setChildren( d, a);
    faces[rhsIndex].setChildren( b, c);
}

LUCID_GIGL_END