#pragma once

#include <lucid/core/Types.h>
#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

///	Statistics
///
///	various, per-frame, measurements
class Statistics
{
public:
	///
	///	totals
	///
	int32_t programs = 0;
	int32_t textures = 0;
	int32_t targets = 0;
	int32_t vertexBuffers = 0;
	int32_t vertexMemory = 0;
	int32_t indexBuffers = 0;
	int32_t indexMemory = 0;

	///
	///	per-frame statistics
	///
	int32_t programChanges = 0;
	int32_t targetChanges = 0;
	int32_t drawCalls = 0;

	Statistics() = default;

	virtual ~Statistics() = default;
};

LUCID_GAL_END