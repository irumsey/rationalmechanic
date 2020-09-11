#include "StarCatalog.h"
#include "Utility.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/core/FileReader.h>

namespace core = ::lucid::core;
namespace  gal = ::lucid:: gal;
namespace gigl = ::lucid::gigl;

namespace lucid {
namespace orbit {

	StarCatalog::StarCatalog(std::string const &path)
	{
		initialize(path);
	}

	void StarCatalog::initialize(std::string const &path)
	{
		/// test {
		///	just making up a map of spectral type -> value for rendering
		///	TBD: make this correct
		std::unordered_map<char, float32_t> map;
		map['O'] = 0.0f;
		map['B'] = 0.1f;
		map['A'] = 0.2f;
		map['F'] = 0.3f;
		map['G'] = 0.4f;
		map['K'] = 0.5f;
		map['M'] = 0.6f;
		map['R'] = 0.7f;
		map['N'] = 0.8f;
		/// } test

		core::FileReader reader(path);

		size_t count = 0;
		reader.read(count);

		_ordinal.resize(count);

		_mesh = gigl::Resources::get<gigl::Mesh>("content/star.mesh");
		_instances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_STATIC, count, sizeof(gal::Vector4)));

		gal::Vector4 *instances = (gal::Vector4 *)(_instances->lock());
		for (size_t i = 0; i < count; ++i)
		{
			Entry entry = _ordinal[i];

			reader.read(entry.            xno);
			reader.read(entry.           type);
			reader.read(entry.right_ascension);
			reader.read(entry.    declination);
			reader.read(entry.      magnitude);

			gal::Vector4 &instance = instances[i];

			instance.x = map[entry.type[0]];
			instance.y = orbit::cast(entry.right_ascension);
			instance.z = orbit::cast(entry.declination);
			instance.w = entry.magnitude;
		}
		_instances->unlock();
	}

	void StarCatalog::shutdown()
	{
		_mesh.reset();
		_instances.reset();

		_ordinal.clear();
	}

	void StarCatalog::render(lucid::gigl::Context const &context) const
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		std::shared_ptr<gigl::Material> material = _mesh->material();
		material->begin(context);
			pipeline.setVertexStream(1, _instances.get());
			_mesh->drawInstanced(_ordinal.size());
		material->end();
	}

}	///	orbit
}	/// lucid
