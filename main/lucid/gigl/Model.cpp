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

void Model::render(Context const &context) const
{
	for (size_t i = 0; i < _meshes.size(); ++i)
		_meshes[i]->render(context);
}

void Model::renderInstanced(Context const &context, LUCID_GAL::VertexBuffer *instanceStream, int32_t start, int32_t count) const
{
	for (size_t i = 0; i < _meshes.size(); ++i)
	{
		LUCID_GAL_PIPELINE.setVertexStream(1, instanceStream, start);
		_meshes[i]->renderInstanced(context, count);
	}
}

void Model::draw() const
{
	for (size_t i = 0; i < _meshes.size(); ++i)
		_meshes[i]->draw();
}

void Model::drawInstanced(LUCID_GAL::VertexBuffer *instanceStream, int32_t start, int32_t count) const
{
	for (size_t i = 0; i < _meshes.size(); ++i)
	{
		LUCID_GAL_PIPELINE.setVertexStream(1, instanceStream, start);
		_meshes[i]->drawInstanced(count);
	}
}

void Model::initialize(LUCID_CORE::Reader &reader)
{
	shutdown();

	int32_t count = reader.read<int32_t>();
	_meshes.resize(count);

	for (int32_t i = 0; i < count; ++i)
		_meshes[i] = Resources::get<Mesh>(reader.read<std::string>());
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