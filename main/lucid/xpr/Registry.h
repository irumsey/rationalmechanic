#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>

LUCID_XPR_BEGIN

/// Registry
///
/// associates an index and value to a symbol.
class Registry 
{
public:
	Registry() = default;

	virtual ~Registry() = default;

	size_t operator()(std::string const &symbol) const
	{
		return index_of(symbol);
	}

	size_t add(std::string const &symbol, float64_t value = 0.0)
	{
		auto iter = indices.find(symbol);
		if (iter != indices.end())
		{
			size_t index = iter->second;
			set(index, value);
			return index;
		}

		size_t index = entries.size();

		entries.push_back(value);
		indices[symbol] = index;
		symbols[index] = symbol;

		return index;
	}

	void set(size_t index, float64_t value)
	{
		entries[index] = value;
	}

	void set(std::string const &symbol, float64_t value)
	{
		set(index_of(symbol), value);
	}

	size_t index_of(std::string const &symbol) const
	{
		auto const iter = indices.find(symbol);
		if (iter == indices.end())
			return -1;
		return iter->second;
	}

	float64_t value_of(size_t index) const
	{
		return entries[index];
	}

	float64_t value_of(std::string const &symbol) const
	{
		return value_of(index_of(symbol));
	}

	std::string const &symbol_for(size_t index) const
	{
		auto const iter = symbols.find(index);
		if (iter == symbols.end())
			return undefined;
		return iter->second;
	}

private:
	std::string const undefined = "<undef>";

	std::vector<float64_t> entries;
	std::unordered_map<std::string, size_t> indices;
	std::unordered_map<size_t, std::string> symbols;
};

LUCID_XPR_END
