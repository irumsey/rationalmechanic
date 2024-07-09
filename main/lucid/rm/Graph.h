#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/core/Numbers.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/rm/Defines.h>

LUCID_RM_BEGIN

///
///
///
struct Input
{
	size_t i = 0;
	float32_t value = 0.f;

	Input() = default;

	Input(size_t i, float32_t value = 0.f);

};

///
///
///
struct Node
{
	typedef float32_t(*transfer_t)(float32_t x);

	static float32_t    step(float32_t x) { return ((x < 0.f) ? 0.f : 1.f); }
	static float32_t sigmoid(float32_t x) { float32_t y = std::powf(LUCID_CORE::numbers::e<float32_t>, x);  return y / (y + 1.f); }

	transfer_t transfer = sigmoid;

	float32_t bias = 0.f;
	float32_t threshold = 0.f;

	float32_t accum = 0.f;
	float32_t output = 0.f;

	Node() = default;

	Node(float32_t bias, float32_t threshold, transfer_t transfer = sigmoid);

};

///
///
///
class Graph
{
public:
	typedef std::unordered_map<size_t, std::unordered_map<size_t, float32_t> > mapping_t;

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

	float32_t edgeWeight(size_t i, size_t j);

	size_t countDownstream(size_t i) const;

	void pushDownstream(std::list<size_t> &stack, size_t i);
	
	size_t countUpstream(size_t i) const;

	void pushUpstream(std::list<size_t> &stack, size_t i);

	void update(size_t count);

	void reset();

private:
	std::vector<Input> _inputs;
	std::vector< Node>  _nodes;

	// mapping from _nodes, to _nodes
	mapping_t _downstream;	// (from, to) all _nodes "_downstream from" the given node
	mapping_t   _upstream;	// (to, from) all _nodes "_upstream from" the given node

	template<class T> size_t add(std::vector<T> &vec, T const &val);

	LUCID_PREVENT_COPY(Graph);
	LUCID_PREVENT_ASSIGNMENT(Graph);
};

inline size_t Graph::inputCount() const
{
	return _inputs.size();
}

inline Input const &Graph::getInput(size_t index) const
{
	return _inputs[index];
}

inline Input &Graph::getInput(size_t index)
{
	return _inputs[index];
}

inline size_t Graph::addInput(Input const &input)
{
	return add(_inputs, input);
}

inline size_t Graph::nodeCount() const
{
	return _nodes.size();
}

inline Node const &Graph::getNode(size_t index) const
{
	return _nodes[index];
}

inline Node &Graph::getNode(size_t index)
{
	return _nodes[index];
}

inline size_t Graph::addNode(Node const &node)
{
	return add(_nodes, node);
}

inline void Graph::adjustEdge(size_t i, size_t j, float32_t weight)
{
	_downstream[i][j] = weight;
	_upstream[j][i] = weight;
}

inline float32_t Graph::edgeWeight(size_t i, size_t j)
{
	return _downstream[i][j];
}

inline size_t Graph::countDownstream(size_t i) const
{
	auto iter = _downstream.find(i);
	if (iter == _downstream.end())
		return 0;

	return iter->second.size();
}

inline size_t Graph::countUpstream(size_t i) const
{
	auto iter = _upstream.find(i);
	if (iter == _upstream.end())
		return 0;

	return iter->second.size();
}

inline void Graph::pushDownstream(std::list<size_t> &stack, size_t i)
{
	auto push = [&stack](auto const &mapped) { stack.push_front(mapped.first); };
	std::for_each(_downstream[i].begin(), _downstream[i].end(), push);
}

inline void Graph::pushUpstream(std::list<size_t> &stack, size_t i)
{
	auto push = [&stack](auto const &mapped) { stack.push_front(mapped.first); };
	std::for_each(_upstream[i].begin(), _upstream[i].end(), push);
}

template<class T> inline size_t Graph::add(std::vector<T> &vec, T const &val)
{
	size_t index = vec.size();
	vec.push_back(val);
	return index;
}

LUCID_RM_END