#pragma once

#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <lucid/core/Error.h>
#include <lucid/math/Math.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Model.h>

LUCID_GIGL_BEGIN

///	Batched
///
///	Instanced batching
///	Note: first, batches are rendered in the order of batch creation.
///	Note: second, all instances within the batch are rendered in order using the supplied predicate.
class Batched
{
public:
	Batched();

	virtual ~Batched();

	void initialize();

	void shutdown();

	template<typename I, typename Pred> void createBatch(std::shared_ptr<Model> model, size_t maximum, Pred const &predicate);

	template<typename I> void addInstance(std::shared_ptr<Model> model, I const &instance);

	template<typename I, typename Pred> void createBatch(std::shared_ptr<Mesh> mesh, size_t maximum, Pred const &predicate);

	template<typename I> void addInstance(std::shared_ptr<Mesh> mesh, I const &instance);

	void render(Context const &context);

	void clear();

private:
	///	Type
	///
	///	Used to generate a unique number per supplied instance type.
	template<typename I> struct Type
	{
		static size_t ID()
		{
			return size_t(&ID);
		}
	};

	///	Key
	///
	///	Hashes a key value using the mesh and instance type.
	/// 
	/// Note: the shared pointer keeps the mesh instance alive.
	///	this allows the system to "register" the batch using createBatch()
	/// then not "worry" about the mesh "going away".
	/// 
	/// See: gigl::Resources
	struct Key
	{
		std::shared_ptr<Mesh> mesh;
		size_t iid = 0;

		Key(std::shared_ptr<Mesh> mesh, size_t iid)
			: mesh(mesh)
			, iid(iid)
		{
		}

		size_t mid() const
		{
			return size_t(mesh.get());
		}
	};

	struct BatchHash
	{
		size_t operator()(Key const &key) const
		{
			return ((0xffff0000 & (key.iid << 16)) | (0x0000ffff & key.mid()));
		}
	};

	struct BatchEqual
	{
		bool operator()(Key const &lhs, Key const &rhs) const
		{
			return (lhs.iid == rhs.iid) && (lhs.mid() == rhs.mid());
		}
	};

	///
	///
	///
	struct BatchBase
	{
		BatchBase() = default;

		virtual ~BatchBase() = default;

		virtual void clear() = 0;

		virtual void push(void const *opaque) = 0;

		virtual void render(Context const &context, std::shared_ptr<Mesh> mesh) = 0;
	};

	typedef std::vector<Key> key_vec_t;
	key_vec_t _order;

	typedef std::unordered_map<Key, BatchBase*, BatchHash, BatchEqual> batch_map_t;
	batch_map_t _batches;

	///
	///
	///
	template<typename I, typename Pred> struct Batch : public BatchBase
	{
		size_t maximum = 0;

		Pred predicate;

		std::vector<I> instances;
		std::shared_ptr<LUCID_GAL::VertexBuffer> batch;

		Batch(size_t maximum, Pred const &predicate)
			: maximum(maximum)
			, predicate(predicate)
		{
			batch.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, int32_t(maximum), sizeof(I)));
		}

		virtual ~Batch() = default;

		virtual void clear() override
		{
			instances.clear();
		}

		virtual void push(void const *opaque) override
		{
			I const &instance = *((I const *)opaque);
			instances.push_back(instance);
		}

		virtual void render(Context const &context, std::shared_ptr<Mesh> mesh) override
		{
			std::sort(instances.begin(), instances.end(), predicate);

			size_t totalCount = instances.size();
			size_t index = 0;

			std::shared_ptr<Material> material = mesh->material();
			material->begin(context);
			while (index < totalCount)
			{
				size_t count = LUCID_MATH::min(totalCount - index, maximum);

				::memcpy(batch->lock_as<I>(), &instances[index], count * sizeof(I));
				batch->unlock();

				LUCID_GAL_PIPELINE.setVertexStream(1, batch.get());
				mesh->drawInstanced(int32_t(count));

				index += count;
			}
			material->end();
		}
	};
};

template<typename I, typename Pred> inline void Batched::createBatch(std::shared_ptr<Model> model, size_t maximum, Pred const &predicate)
{
	Model::mesh_vec_t const &meshes = model->_meshes;
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		createBatch<I,Pred>(meshes[i], maximum, predicate);
	}
}

template<typename I> inline void Batched::addInstance(std::shared_ptr<Model> model, I const &instance)
{
	Model::mesh_vec_t const &meshes = model->_meshes;
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		addInstance<I>(meshes[i], instance);
	}
}

template<typename I, typename Pred> inline void Batched::createBatch(std::shared_ptr<Mesh> mesh, size_t maximum, Pred const &predicate)
{
	Key key = Key(mesh, Type<I>::ID());
	if (_batches.end() != _batches.find(key))
		return;

	_order.push_back(key);
	_batches.insert(std::make_pair(key, new Batch<I,Pred>(maximum, predicate)));
}

template<typename I> inline void Batched::addInstance(std::shared_ptr<Mesh> mesh, I const &instance)
{
	auto iter = _batches.find(Key(mesh, Type<I>::ID()));
	LUCID_VALIDATE(iter != _batches.end(), "batched mesh/instance pair not registered");

	iter->second->push(&instance);
}

LUCID_GIGL_END