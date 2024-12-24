#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>

LUCID_XPR_BEGIN

/// Registry
///
/// variable name registry.  associates an index and value to a symbol.
class Registry 
{
public:
	Registry() = default;

	virtual ~Registry() = default;

	uint64_t operator()(std::string const &symbol) const
	{
		return index_of(symbol);
	}

	uint64_t add(std::string const &symbol, float64_t value = 0.0)
	{
		auto iter = indices.find(symbol);
		if (iter != indices.end())
		{
			uint64_t index = iter->second;
			set(index, value);
			return index;
		}

		uint64_t index = uint64_t(entries.size());

		entries.push_back(value);
		indices[symbol] = index;
		symbols[index] = symbol;

		return index;
	}

	void set(uint64_t index, float64_t value)
	{
		entries[size_t(index)] = value;
	}

	void set(std::string const &symbol, float64_t value)
	{
		set(index_of(symbol), value);
	}

	uint64_t index_of(std::string const &symbol) const
	{
		auto const iter = indices.find(symbol);
		if (iter == indices.end())
			return -1;
		return iter->second;
	}

	float64_t value_of(uint64_t index) const
	{
		return entries[index];
	}

	float64_t value_of(std::string const &symbol) const
	{
		return value_of(index_of(symbol));
	}

	std::string const &symbol_for(uint64_t index) const
	{
		auto const iter = symbols.find(index);
		if (iter == symbols.end())
			return undefined;
		return iter->second;
	}

private:
	std::string const undefined = "<undef>";

	std::vector<float64_t> entries;
	std::unordered_map<std::string, uint64_t> indices;
	std::unordered_map<uint64_t, std::string> symbols;
};

LUCID_XPR_END
