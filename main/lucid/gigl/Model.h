#pragma once

#include <memory>
#include <vector>
#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Identity.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Mesh.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GIGL_BEGIN

class Batched;

///	Model
///
///	Collection of meshes that make up a more complicated object.
/// 
///	Note: one caveat is that all meshes must use the same per-instance-vertex.
class Model final
{
public:
	Model(LUCID_CORE::Reader &reader);

	virtual ~Model();

	LUCID_CORE::Identity const &identity() const;

	static Model *create(std::string const &path);

	static Model *create(LUCID_CORE::Reader &reader);

private:
	typedef std::shared_ptr<Mesh> mesh_ptr_t;
	typedef std::vector<mesh_ptr_t> mesh_vec_t;

	friend class Batched;

	LUCID_CORE::Identity _identity;
	mesh_vec_t _meshes;

	void initialize(LUCID_CORE::Reader &reader);

	void shutdown();

	LUCID_PREVENT_COPY(Model);
	LUCID_PREVENT_ASSIGNMENT(Model);
};

inline LUCID_CORE::Identity const &Model::identity() const
{
	return _identity;
}

LUCID_GIGL_END