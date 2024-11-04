#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <lucid/core/Error.h>
#include <lucid/core/Identity.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Types.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_BEGIN

class VertexFormat;
class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

///	Geometry
///
///
class Geometry final
{
public:
	Geometry(LUCID_CORE::Reader &reader);

	virtual ~Geometry();

	LUCID_CORE::Identity const &identity() const;

	LUCID_GAL::Pipeline::TOPOLOGY topology() const;

	uint32_t vertexCount() const;

	uint32_t indexCount() const;

	uint32_t primitiveCount() const;

	void draw() const;

	/// Note: assumes per-instance stream has been set
	void drawInstanced(int32_t count) const;

	static Geometry *create(std::string const &path);

	static Geometry *create(LUCID_CORE::Reader &reader);

private:
	LUCID_CORE::Identity const _identity;

	LUCID_GAL::Pipeline::TOPOLOGY _topology = LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_LIST;

	std::unique_ptr<LUCID_GAL::VertexFormat> _format;
	std::unique_ptr<LUCID_GAL::VertexBuffer> _vertices;
	std::unique_ptr<LUCID_GAL::IndexBuffer> _indices;

	void initialize(LUCID_CORE::Reader &reader);

	void shutdown();

	LUCID_PREVENT_COPY(Geometry);
	LUCID_PREVENT_ASSIGNMENT(Geometry);
};

inline LUCID_CORE::Identity const &Geometry::identity() const
{
	return _identity;
}

inline LUCID_GAL::Pipeline::TOPOLOGY Geometry::topology() const
{
	return _topology;
}

inline uint32_t Geometry::primitiveCount() const
{
	switch (_topology)
	{
	case LUCID_GAL::Pipeline::TOPOLOGY_POINT_LIST:
		return indexCount();
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_LINE_STRIP:
		return indexCount() - 1;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_LINE_LIST:
		return indexCount() / 2;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_STRIP:
		return indexCount() - 2;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_LIST:
		return indexCount() / 3;
		break;
	case LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_ADJACENCY:
		return indexCount() / 6;
		break;
	default:
		break;
	}

	return 0;
}

LUCID_GIGL_END
