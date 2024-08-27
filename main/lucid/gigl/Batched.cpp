#include "Batched.h"
#include <algorithm>

LUCID_GIGL_BEGIN

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
	_order.clear();
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
		iter->second->clear();
}

LUCID_GIGL_END