#include "Graph.h"

namespace /* anonymous */
{

	struct Propagate
	{
		std::vector<Node> &nodes;

		Propagate(std::vector<Node> &nodes)
			: nodes(nodes)
		{
		}

		inline void operator()(Graph::mapping_t::value_type const &src) const
		{
			auto accumulate = [this, src](auto const &dst) { nodes[dst.first].accum += dst.second * nodes[src.first].output; };
			std::for_each(src.second.begin(), src.second.end(), accumulate);
		}
	};

}

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
	auto feed = [this](Input const &input) { nodes[input.i].accum += input.value;  };
	auto transfer = [](Node &node) { node.output = (node.accum > node.threshold) ? node.transfer(node.accum) : 0.f; node.accum = node.bias; };

	while (count != 0)
	{
		std::for_each(inputs.begin(), inputs.end(), feed);
		std::for_each(downstream.begin(), downstream.end(), Propagate(nodes));
		std::for_each(nodes.begin(), nodes.end(), transfer);
		--count;
	}
}

void Graph::reset()
{
	inputs.clear();
	nodes.clear();

	upstream.clear();
	downstream.clear();
}
