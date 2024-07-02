#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <lucid/gal/Defines.h>

#define LUCID_GAL_D3D11 ::lucid::gal::d3d11

#define LUCID_GAL_D3D11_BEGIN LUCID_GAL_BEGIN namespace d3d11 {
#define LUCID_GAL_D3D11_END } LUCID_GAL_END
