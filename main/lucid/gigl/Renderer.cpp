#include "Renderer.h"
#include "Stage.h"
#include <lucid/core/FileReader.h>

namespace lucid {
namespace gigl {

	Renderer::Renderer(std::string const &path)
	{
		initialize(core::FileReader(path));
	}

	Renderer::Renderer(core::Reader &reader)
	{
		initialize(reader);
	}

	Renderer::~Renderer()
	{
		shutdown();
	}

	Stage *Renderer::stage(std::string const &name) const
	{
		///	linear search. if this becomes an issue, use hash table...
		for (Stage *stage = _stage; stage; stage = stage->next())
			if (name == stage->name())
				return stage;
		return nullptr;
	}

	void Renderer::render(Context const &context) const
	{
		for (Stage *stage = _stage; stage; stage = stage->next())
			stage->render(context);
	}

	void Renderer::initialize(core::Reader &reader)
	{
		shutdown();
		try
		{

		}
		catch (...)
		{
			shutdown();
			throw;
		}
	}

	void Renderer::shutdown()
	{
		delete _stage;
		_stage = nullptr;
	}

}	///	gigl
}	///	lucid
