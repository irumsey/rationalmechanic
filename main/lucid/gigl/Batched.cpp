#include "Batched.h"
#include <algorithm>

namespace lucid {
namespace gigl {

	size_t Batched::TypeID::counter = 0;

	Batched::Batched()
	{
	}

	Batched::~Batched()
	{
		shutdown();
	}

	void Batched::initialize()
	{
		shutdown();
		/// TBD: nothing to do???
	}

	void Batched::shutdown()
	{
		for (auto iter = _batches.begin(); iter != _batches.end(); ++iter)
			delete iter->second;
		_batches.clear();
	}

	void Batched::render(Context const &context)
	{
		for (auto iter = _order.begin(); iter != _order.end(); ++iter)
		{
			Key const &key = *iter;
			_batches[key]->render(context, key.mesh);
		}
	}

	void Batched::clear()
	{
		for (auto iter = _batches.begin(); iter != _batches.end(); ++iter)
		{
			Key const &key = iter->first;
			BatchBase *batch = iter->second;

			batch->clear();
		}
	}

}	///	gigl
}	///	lucid