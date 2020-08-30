#include "Octree.h"
#include "Frame.h"
#include <lucid/math/Constants.h>

namespace math = ::lucid::math;
namespace orbit = ::lucid::orbit;

namespace /* anonymous */ {

}

namespace lucid {
namespace orbit {

	Octree::Octree(aabb3_t const &aabb, size_t depth)
	{
		initialize(aabb, depth);
	}

	Octree::~Octree()
	{
		shutdown();
	}

	void Octree::initialize(aabb3_t const &aabb, size_t limit)
	{
		shutdown();

		_database.insert(std::make_pair(NODE_ROOT, Node(NODE_ROOT, aabb)));
		subdivide(NODE_ROOT, 1, limit);
	}

	void Octree::shutdown()
	{
		_database.clear();
	}

	void Octree::subdivide(size_t code, size_t depth, size_t limit)
	{
		if (!(depth < limit))
			return;

		auto iter = _database.find(code);
		LUCID_VALIDATE(iter != _database.end(), "invalid spatial database code given");

		Node const &parent = iter->second;
		vector3_t center = parent.aabb.center();

		vector3_t const corner[] =
		{
			vector3_t(parent.aabb.min.x, parent.aabb.min.y, parent.aabb.min.z),
			vector3_t(parent.aabb.max.x, parent.aabb.min.y, parent.aabb.min.z),
			vector3_t(parent.aabb.min.x, parent.aabb.max.y, parent.aabb.min.z),
			vector3_t(parent.aabb.max.x, parent.aabb.max.y, parent.aabb.min.z),
			vector3_t(parent.aabb.min.x, parent.aabb.min.y, parent.aabb.max.z),
			vector3_t(parent.aabb.max.x, parent.aabb.min.y, parent.aabb.max.z),
			vector3_t(parent.aabb.min.x, parent.aabb.max.y, parent.aabb.max.z),
			vector3_t(parent.aabb.max.x, parent.aabb.max.y, parent.aabb.max.z),
		};

		auto edgeCenter = [&corner](size_t i, size_t j) { return scalar_t(0.5) * (corner[i] + corner[j]); };
		auto faceCenter = [&corner](size_t i, size_t j, size_t k, size_t l) { return scalar_t(0.25) * (corner[i] + corner[j] + corner[k] + corner[l]); };

		aabb3_t const aabb[] = {
			aabb3_t(             corner[0],                 center),
			aabb3_t(edgeCenter(      0, 1), faceCenter(1, 3, 5, 7)),
			aabb3_t(edgeCenter(      0, 2), faceCenter(2, 3, 6, 7)),
			aabb3_t(faceCenter(0, 1, 2, 3), edgeCenter(      3, 7)),
			aabb3_t(edgeCenter(      0, 4), faceCenter(4, 5, 6, 7)),
			aabb3_t(faceCenter(0, 1, 4, 5), edgeCenter(      5, 7)),
			aabb3_t(faceCenter(0, 2, 4, 6), edgeCenter(      6, 7)),
			aabb3_t(                center,              corner[7]),
		};

		for (size_t index = 0; index < 8; ++index)
		{
			size_t childCode = getChildCode(parent.code, index);

			_database.insert(std::make_pair(childCode, Node(childCode, aabb[index])));
			subdivide(childCode, depth + 1, limit);
		}
	}

}	///	orbit
}	///	lucid
