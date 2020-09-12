#include "StarCatalog.h"
#include "Utility.h"
#include <lucid/core/FileReader.h>

namespace core = ::lucid::core;

namespace lucid {
namespace orbit {

	void StarCatalog::initialize(std::string const &path)
	{
		core::FileReader reader(path);

		size_t count = 0;
		reader.read(count);

		_ordinal.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			Entry &entry = _ordinal[i];

			reader.read(entry.            xno);
			reader.read(entry.           type);
			reader.read(entry.right_ascension);
			reader.read(entry.    declination);
			reader.read(entry.      magnitude);
		}
	}

	void StarCatalog::shutdown()
	{
		_ordinal.clear();
	}

}	///	orbit
}	/// lucid
