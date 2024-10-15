#pragma once

#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/Math.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

///	Heightmap
/// 
/// Note: using 8 bit values for height for now, migrate to 16 when
/// an actual file format is specified.
class Heightmap
{
public:
	typedef LUCID_MATH::AABB<LUCID_GAL::Scalar, 2> Area;

    struct Tile
    {
        static size_t const INVALID_INDEX = size_t(-1);

        size_t id = INVALID_INDEX;

        Area area;
        uint8_t h[2] = { 255, 0, };

        Tile() = default;

        Tile(size_t id, Area const &area)
            : id(id)
            , area(area)
        {
        }

        ~Tile() = default;

    };

	Heightmap();

    Heightmap(size_t width, size_t height, uint8_t value = 0);

	virtual ~Heightmap();

	void initialize(size_t width, size_t height, uint8_t value = 0);

	void shutdown();

	uint8_t &operator()(size_t x, size_t y);

	uint8_t const &operator()(size_t x, size_t y) const;

	uint8_t &at(size_t x, size_t y);

	uint8_t const &at(size_t x, size_t y) const;

    void process(size_t depth, uint8_t tolerance);

    Tile const &filter(Area const &area) const;

private:
	size_t _width = 0;
	size_t _height = 0;
	size_t _size = 0;

	uint8_t *_data = nullptr;

	std::unordered_map<size_t, Tile> _tiles;

    size_t extract_parent_id(size_t cid) const;

    size_t hash_child_id(size_t pid, size_t index) const;

    bool contains(Tile const &tile, Area const &area) const;

    void createTile(size_t id, LUCID_GAL::Vector2 const &min, LUCID_GAL::Vector2 const &max);
 
    void removeChildren(size_t pid);

    void subdivide(Tile &tile, size_t depth, uint8_t tolerance);

    void subdivide(Tile const &tile);

    Tile const &filter(Tile const &tile, Area const &area) const;

	LUCID_PREVENT_COPY(Heightmap);
	LUCID_PREVENT_ASSIGNMENT(Heightmap);
};

inline uint8_t &Heightmap::operator()(size_t x, size_t y)
{
	return at(x, y);
}

inline uint8_t const &Heightmap::operator()(size_t x, size_t y) const
{
	return at(x, y);
}

inline uint8_t &Heightmap::at(size_t x, size_t y)
{
	assert((x < _width) && "index out of bounds");
	assert((y < _height) && "index out of bounds");
	return _data[y * _width + x];
}

inline uint8_t const &Heightmap::at(size_t x, size_t y) const
{
	assert((x < _width) && "index out of bounds");
	assert((y < _height) && "index out of bounds");
	return _data[y * _width + x];
}

inline size_t Heightmap::extract_parent_id(size_t cid) const
{
    return cid >> 2;
}

inline size_t Heightmap::hash_child_id(size_t pid, size_t index) const
{
    return (pid << 2) | (0x03 & index);
}

inline bool Heightmap::contains(Tile const &tile, Area const &area) const
{
    return LUCID_MATH::looselyContains(tile.area, area);
}

LUCID_GIGL_END
