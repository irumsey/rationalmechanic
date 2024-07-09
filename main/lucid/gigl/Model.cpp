#include "Model.h"
#include "Resources.h"
#include <lucid/core/FileReader.h>
#include <lucid/core/Reader.h>

LUCID_GIGL_BEGIN

Model::Model(LUCID_CORE::Reader &reader)
{
	initialize(reader);
}

Model::~Model()
{
	shutdown();
}

void Model::initialize(LUCID_CORE::Reader &reader)
{
	shutdown();

	int32_t count = reader.read<int32_t>();
	_meshes.resize(count);

	for (int32_t i = 0; i < count; ++i)
	{
		std::string path = reader.read<std::string>();

		_meshes[i] = Resources::get<Mesh>(path);
	}
}

void Model::shutdown()
{
	_meshes.clear();
}

Model *Model::create(std::string const &path)
{
	return create(LUCID_CORE::FileReader(path));
}

Model *Model::create(LUCID_CORE::Reader &reader)
{
	return new Model(reader);
}

LUCID_GIGL_END