#include "StarCatalog.h"
#include "Utility.h"
#include <lucid/core/FileReader.h>

namespace core = ::lucid::core;

namespace lucid {
namespace orbit {

	void StarCatalog::initialize(std::string const &path)
	{
		core::Reader &reader = core::FileReader(path);

		int32_t count = reader.read<int32_t>();

		_ordinal.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			Entry &entry = _ordinal[i];

			entry.            xno = reader.read<int32_t>();
			entry.           type = reader.read<std::string>();
			entry.right_ascension = reader.read<scalar_t>();
			entry.    declination = reader.read<scalar_t>();
			entry.      magnitude = reader.read<float32_t>();
		}
	}

	void StarCatalog::shutdown()
	{
		_ordinal.clear();
	}

}	///	orbit
}	/// lucid
