#include "Heightmap.h"

LUCID_GIGL_BEGIN

Heightmap::Heightmap()
{
}

Heightmap::Heightmap(size_t width, size_t height, uint8_t value)
{
    initialize(width, height, value);
}

Heightmap::~Heightmap()
{
	shutdown();
}

void Heightmap::initialize(size_t width, size_t height, uint8_t value)
{
	shutdown();

	_width = width;
	_height = height;
	_size = _width * _height;

	_data = new uint8_t [_size];
	std::fill(_data, _data + _size, value);
}

void Heightmap::shutdown()
{
	_tiles.clear();

	delete [] _data;
	_data = nullptr;

	_width = 0;
	_height = 0;
	_size = 0;
}

void Heightmap::process(size_t depth, uint8_t tolerance)
{
    LUCID_VALIDATE(0 < depth, "invalid depth specified");
    _tiles.clear();

    createTile(1, LUCID_GAL::Vector2(0, 0), LUCID_GAL::Vector2(1, 1));
    subdivide(_tiles[1], depth - 1, tolerance);
}

Heightmap::Tile const &Heightmap::filter(Area const &area) const
{
    auto iter = _tiles.find(1);
    assert((iter != _tiles.end()) && "consistency error");

    return filter(iter->second, area);
}

void Heightmap::createTile(size_t id, LUCID_GAL::Vector2 const &min, LUCID_GAL::Vector2 const &max)
{
    assert((_tiles.end() == _tiles.find(id)) && "tile already exists");
    _tiles.insert(std::make_pair(id, Tile(id, Area(min, max))));
}

void Heightmap::removeChildren(size_t pid)
{
    _tiles.erase(hash_child_id(pid, 0));
    _tiles.erase(hash_child_id(pid, 1));
    _tiles.erase(hash_child_id(pid, 2));
    _tiles.erase(hash_child_id(pid, 3));
}

void Heightmap::subdivide(Tile &tile, size_t depth, uint8_t tolerance)
{
    if (0 == depth)
    {
        size_t x0 = size_t(tile.area.min.x * _width);
        size_t x1 = size_t(tile.area.max.x * _width);

        size_t y0 = size_t(tile.area.min.y * _height);
        size_t y1 = size_t(tile.area.max.y * _height);

        for (size_t x = x0; x < x1; ++x)
        {
            for (size_t y = y0; y < y1; ++y)
            {
                uint8_t h = at(x, y);
                tile.h[0] = std::min(tile.h[0], h);
                tile.h[1] = std::max(tile.h[1], h);
            }
        }

        return;
    }

    subdivide(tile);

    for (size_t i = 0; i < 4; ++i)
    {
        size_t cid = hash_child_id(tile.id, i);

        auto iter = _tiles.find(cid);
        assert(iter != _tiles.end());

        Tile &child = iter->second;
        subdivide(child, depth - 1, tolerance);

        tile.h[0] = std::min(tile.h[0], child.h[0]);
        tile.h[1] = std::max(tile.h[1], child.h[1]);
    }

    if ((tile.h[1] - tile.h[0]) < tolerance)
    {
        removeChildren(tile.id);
        tile.h[0] = tile.h[1] = ((tile.h[1] - tile.h[0]) >> 1);
    }
}

void Heightmap::subdivide(Tile const &tile)
{
    LUCID_GAL::Vector2 const &min = tile.area.min;
    LUCID_GAL::Vector2 const &max = tile.area.max;

    LUCID_GAL::Scalar cx = 0.5f * (max.x + min.x);
    LUCID_GAL::Scalar cy = 0.5f * (max.y + min.y);

    createTile(hash_child_id(tile.id, 0), LUCID_GAL::Vector2(cx, cy), max);
    createTile(hash_child_id(tile.id, 1), LUCID_GAL::Vector2(min.x, cy), LUCID_GAL::Vector2(cx, max.y));
    createTile(hash_child_id(tile.id, 2), min, LUCID_GAL::Vector2(cx, cy));
    createTile(hash_child_id(tile.id, 3), LUCID_GAL::Vector2(cx, min.y), LUCID_GAL::Vector2(max.x, cy));
}

Heightmap::Tile const &Heightmap::filter(Tile const &tile, Area const &area) const
{
    assert(contains(tile, area) && "consistency error");

    for (size_t i = 0; i < 4; ++i)
    {
        size_t cid = hash_child_id(tile.id, i);

        auto iter = _tiles.find(cid);
        if (iter == _tiles.end())
            return tile;

        Tile const &child = iter->second;
        if (contains(child, area))
            return filter(child, area);
    }

    return tile;
}

LUCID_GIGL_END
