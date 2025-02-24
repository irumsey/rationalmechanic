#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <winerror.h>
#include <comdef.h>
#include <lucid/core/Error.h>
#include <lucid/core/Unserializer.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

///
///
///
inline void GAL_VALIDATE_HRESULT(HRESULT hr, std::string const &msg)
{
	if (!SUCCEEDED(hr))
	{
		_com_error err(hr);
		TCHAR const *wstr = err.ErrorMessage();

		std::string reason;
		for (TCHAR const *ptr = wstr; 0 != *ptr; ++ptr) { reason.push_back((char const)(*ptr)); }

		///	Note: the file and line value in core::Error will not be useful.
		LUCID_THROW(msg);
	}
}

LUCID_GAL_D3D11_BEGIN

template<class T> inline void safeRelease(T *&ptr)
{
	if (nullptr != ptr)
	{
		ptr->Release();
	}
	ptr = nullptr;
}

///	readEnum
///
///
template<class T, int32_t N> inline T readEnum(LUCID_CORE::Unserializer &reader, T const (&lookup)[N])
{
	int32_t index = reader.read<int32_t>();
	LUCID_VALIDATE(index < N, "gal enumeration index out of bounds");

	return lookup[index];
}

LUCID_GAL_D3D11_END
