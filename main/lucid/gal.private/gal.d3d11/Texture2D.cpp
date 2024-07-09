#include "Texture2D.h"
#include "TextureUtility.h"
#include "System.h"
#include "Pipeline.h"
#include "Utility.h"
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <vector>
#include <fstream>

LUCID_GAL_BEGIN

Texture2D *Texture2D::create(std::string const &path)
{
	return new LUCID_GAL_D3D11::Texture2D(path);
}

Texture2D *Texture2D::create(LUCID_CORE::Reader &reader)
{
	return new LUCID_GAL_D3D11::Texture2D(reader);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

Texture2D::Texture2D(std::string const &path)
{
	try
	{
		initialize(path);
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

Texture2D::Texture2D(LUCID_CORE::Reader &reader)
{
	try
	{
		initialize(reader.read<std::string>());
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

Texture2D::~Texture2D()
{
	shutdown();
}

void Texture2D::initialize(std::string const &path)
{
	std::ifstream stream(path.c_str(), std::ios::in | std::ios::binary);
	LUCID_VALIDATE(stream.is_open(), "unable to open: " + path);

	stream.seekg(0, std::ios::end);
	size_t size = (size_t)stream.tellg();
	stream.seekg(0, std::ios::beg);

	std::vector<uint8_t> data(size);
	stream.read((char*)(&data[0]), size);

	::CreateDDSTextureFromMemory(d3d11ConcreteDevice, &data[0], size, &_d3dResource, &_d3dResourceView);

	++galConcreteStatistic(textures);
}

void Texture2D::shutdown()
{
	safeRelease(_d3dResource);
	safeRelease(_d3dResourceView);

	++galConcreteStatistic(textures);
}

LUCID_GAL_D3D11_END
