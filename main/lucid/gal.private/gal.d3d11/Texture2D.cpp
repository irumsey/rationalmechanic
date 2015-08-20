#include "Texture2D.h"
#include "TextureUtility.h"
#include "System.h"
#include "Pipeline.h"
#include "Utility.h"
#include <vector>
#include <fstream>

///
///
///
namespace lucid {
namespace gal {

	Texture2D *Texture2D::create(std::string const &path)
	{
		return new ::lucid::gal::d3d11::Texture2D(path);
	}

	Texture2D *Texture2D::create(::lucid::core::Reader &reader)
	{
		return new ::lucid::gal::d3d11::Texture2D(reader);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	Texture2D::Texture2D(std::string const &path)
	{
		try
		{
			initialize(path);
		}
		catch (...)
		{
			shutdown();
			throw;
		}
	}

	Texture2D::Texture2D(::lucid::core::Reader &reader)
	{
		try
		{
			initialize(reader.read<std::string>());
		}
		catch (...)
		{
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

}	///	d3d11
}	///	gal
}	///	lucid