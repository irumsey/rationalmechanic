#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/math/Constants.h>

struct Input
{
	size_t i = 0;
	float32_t value = 0.f;

	Input() = default;

	Input(size_t i, float32_t value = 0.f);

};

struct Node
{
	typedef float32_t(*function_t)(float32_t x);

	static float32_t    step(float32_t x) { return ((x < 0.f) ? 0.f : 1.f); }
	static float32_t sigmoid(float32_t x) { float32_t y = std::powf(::lucid::math::constants::e<float32_t>(), x);  return y / (y + 1.f); }

	function_t transfer = sigmoid;

	float32_t bias = 0.f;
	float32_t threshold = 0.f;

	float32_t accum = 0.f;
	float32_t output = 0.f;

	Node() = default;

	Node(float32_t bias, float32_t threshold, function_t transfer = sigmoid);

};

class Graph
{
public:
	Graph() = default;

	virtual ~Graph() = default;

	size_t inputCount() const;

	Input const &getInput(size_t index) const;

	Input &getInput(size_t index);

	size_t addInput(Input const &input);

	size_t nodeCount() const;

	Node const &getNode(size_t index) const;

	Node &getNode(size_t index);

	size_t addNode(Node const &node);

	void adjustEdge(size_t i, size_t j, float32_t weight);

	void pushDownstream(std::list<size_t> &stack, size_t i);
	 
	void pushUpstream(std::list<size_t> &stack, size_t i);

	void update(size_t count);

	void reset();

private:
	typedef std::unordered_map<size_t, std::unordered_map<size_t, float32_t> > mapping_t;

	std::vector<Input> inputs;
	std::vector< Node>  nodes;

	// mapping from nodes, to nodes
	mapping_t downstream;	// (from, to) all nodes "downstream from" the given node
	mapping_t   upstream;	// (to, from) all nodes "upstream from" the given node

	template<class T> size_t add(std::vector<T> &vec, T const &val)
	{
		size_t index = vec.size();
		vec.push_back(val);
		return index;
	}

};

inline size_t Graph::inputCount() const
{
	return inputs.size();
}

inline Input const &Graph::getInput(size_t index) const
{
	return inputs[index];
}

inline Input &Graph::getInput(size_t index)
{
	return inputs[index];
}

inline size_t Graph::addInput(Input const &input)
{
	return add(inputs, input);
}

inline size_t Graph::nodeCount() const
{
	return nodes.size();
}

inline Node const &Graph::getNode(size_t index) const
{
	return nodes[index];
}

inline Node &Graph::getNode(size_t index)
{
	return nodes[index];
}

inline size_t Graph::addNode(Node const &node)
{
	return add(nodes, node);
}

inline void Graph::adjustEdge(size_t i, size_t j, float32_t weight)
{
	downstream[i][j] = weight;
	upstream[j][i] = weight;
}

inline void Graph::pushDownstream(std::list<size_t> &stack, size_t i)
{
	auto push = [&stack](auto const &mapped) { stack.push_front(mapped.first); };
	std::for_each(downstream[i].begin(), downstream[i].end(), push);
}

inline void Graph::pushUpstream(std::list<size_t> &stack, size_t i)
{
	auto push = [&stack](auto const &mapped) { stack.push_front(mapped.first); };
	std::for_each(upstream[i].begin(), upstream[i].end(), push);
}
