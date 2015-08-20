#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Types.h>

///
///	Provided by Microsoft
///

void CreateDDSTextureFromMemory
(
	_In_ ID3D11Device* d3dDevice,
	_In_reads_bytes_(ddsDataSize) const byte* ddsData,
	_In_ size_t ddsDataSize,
	_Out_opt_ ID3D11Resource** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_In_ size_t maxsize = 0
);
