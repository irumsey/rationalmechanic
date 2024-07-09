#pragma once

#include <memory>
#include <string>
#include <vector>
#include <lucid/core/Identity.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gigl/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_BEGIN

class Program;

class VertexFormat;
class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

///
///
///

class Context;
class Material;
class Geometry;

///	Mesh
///
///	a gigl mesh is defined as: a material and geometry.
///
///	note: notice the complete lack of any specific vertex
///	structure.  mesh is entirely data driven which includes the
///	vertex layout.  it would be improper to impose a vertex
///	structure here.
class Mesh final
{
public:
	Mesh(core::Reader &reader);

	virtual ~Mesh();

	core::Identity const &identity() const;

	std::shared_ptr<LUCID_GAL::Program> program() const;

	std::shared_ptr<Material> material() const;

	LUCID_GAL::Pipeline::TOPOLOGY topology() const;

	uint32_t vertexCount() const;

	uint32_t indexCount() const;

	uint32_t primitiveCount() const;

	void render(Context const &context) const;

	///	this assumes the per-instance stream(s) are already set.
	void renderInstanced(Context const &context, int32_t count) const;

	void draw() const;

	///	this assumes the per-instance stream(s) are already set.
	void drawInstanced(int32_t count) const;

	static Mesh *create(std::string const &path);

	static Mesh *create(LUCID_CORE::Reader &reader);

private:
	core::Identity const _identity;

	std::shared_ptr<LUCID_GAL::Program> _program;

	std::shared_ptr<Material> _material;
	std::shared_ptr<Geometry> _geometry;

	void initialize(LUCID_CORE::Reader &reader);

	void shutdown();

	LUCID_PREVENT_COPY(Mesh);
	LUCID_PREVENT_ASSIGNMENT(Mesh);
};

inline core::Identity const &Mesh::identity() const
{
	return _identity;
}

inline std::shared_ptr<LUCID_GAL::Program> Mesh::program() const
{
	return _program;
}

inline std::shared_ptr<Material> Mesh::material() const
{
	return _material;
}

LUCID_GIGL_END