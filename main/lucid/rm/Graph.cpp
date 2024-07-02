#include "Graph.h"

LUCID_RM_BEGIN

//
//
//

struct Propagate
{
	std::vector<Node> &_nodes;

	Propagate(std::vector<Node> &_nodes)
		: _nodes(_nodes)
	{
	}

	inline void operator()(Graph::mapping_t::value_type const &src) const
	{
		auto accumulate = [this, src](auto const &dst) { _nodes[dst.first].accum += dst.second * _nodes[src.first].output; };
		std::for_each(src.second.begin(), src.second.end(), accumulate);
	}
};

//
//
//

Input::Input(size_t i, float32_t value)
	: i(i)
	, value(value)
{
}

//
//
//

Node::Node(float32_t bias, float32_t threshold, transfer_t transfer)
	: bias(bias), threshold(threshold)
	, transfer(transfer)
	, accum(bias)
{
}

//
//
//

void Graph::update(size_t count)
{
	auto feed = [this](Input const &input) { _nodes[input.i].accum += input.value;  };
	auto transfer = [](Node &node) { node.output = (node.accum > node.threshold) ? node.transfer(node.accum) : 0.f; node.accum = node.bias; };

	while (count != 0)
	{
		std::for_each(_inputs.begin(), _inputs.end(), feed);
		std::for_each(_downstream.begin(), _downstream.end(), Propagate(_nodes));
		std::for_each(_nodes.begin(), _nodes.end(), transfer);
		--count;
	}
}

void Graph::reset()
{
	_inputs.clear();
	_nodes.clear();

	_upstream.clear();
	_downstream.clear();
}

LUCID_RM_END