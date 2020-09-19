#include "Model.h"
#include "Resources.h"
#include <lucid/core/FileReader.h>

namespace lucid {
namespace gigl {

	Model::Model(core::Reader &reader)
	{
		initialize(reader);
	}

	Model::~Model()
	{
		shutdown();
	}

	void Model::initialize(core::Reader &reader)
	{
		shutdown();

		size_t count = 0;
		reader.read(count);

		_meshes.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			std::string path;
			reader.read(path);

			_meshes[i] = Resources::get<Mesh>(path);
		}
	}

	void Model::shutdown()
	{
		_meshes.clear();
	}

	Model *Model::create(std::string const &path)
	{
		return create(core::FileReader(path));
	}

	Model *Model::create(core::Reader &reader)
	{
		return new Model(reader);
	}

}	///	gigl
}	///	lucid
