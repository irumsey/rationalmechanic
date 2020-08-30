#pragma once

#include <list>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/AABB.h>
#include <lucid/orbit/Types.h>

///
///	
///	

namespace lucid {
namespace orbit {

	class Frame;

	///	Octree
	///
	///	Morton (Z curve) coded nodes
	class Octree
	{
	public:
		size_t const NODE_INVALID = 0;
		size_t const    NODE_ROOT = 1;

		Octree() = default;

		Octree(aabb3_t const &aabb, size_t depth);

		virtual ~Octree();

		void initialize(aabb3_t const &aabb, size_t depthLimit);

		void shutdown();

	private:
		///	Node
		///
		///
		struct Node
		{
			size_t code = 0;
			aabb3_t aabb;
			std::list<Frame *> frames;

			Node() = delete;

			Node(size_t code, aabb3_t const &aabb)
				: code(code)
				, aabb(aabb)
			{
			}

			~Node() = default;
		};

		typedef std::unordered_map<size_t, Node> database_t;

		database_t _database;

		size_t getParentCode(size_t childCode) const;

		size_t getChildCode(size_t parentCode, size_t chidIndex) const;

		void subdivide(size_t code, size_t depth, size_t limit);

		LUCID_PREVENT_COPY(Octree);
		LUCID_PREVENT_ASSIGNMENT(Octree);
	};

	inline size_t Octree::getParentCode(size_t childCode) const
	{
		return (childCode >> 3);
	}

	inline size_t Octree::getChildCode(size_t parentCode, size_t childIndex) const
	{
		return ((parentCode << 3) | (0x07 & childIndex));
	}

}	///	orbit
}	///	lucid
