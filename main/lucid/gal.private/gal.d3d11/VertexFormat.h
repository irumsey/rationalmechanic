#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_D3D11_BEGIN

///
///
///
class VertexFormat : public LUCID_GAL::VertexFormat
{
public:
	VertexFormat(std::vector<LUCID_GAL::VertexElement> const &layout);

	virtual ~VertexFormat();

	virtual std::vector<LUCID_GAL::VertexElement> const &layout() const override;

	ID3D11InputLayout *d3dLayout() const;

private:
	std::vector<LUCID_GAL::VertexElement> _layout;
	ID3D11InputLayout *_d3dLayout = nullptr;

	void initialize(std::vector<LUCID_GAL::VertexElement> const &layout);

	void shutdown();

	LUCID_PREVENT_COPY(VertexFormat);
	LUCID_PREVENT_ASSIGNMENT(VertexFormat);
};

inline std::vector<LUCID_GAL::VertexElement> const &VertexFormat::layout() const
{
	return _layout;
}

inline ID3D11InputLayout *VertexFormat::d3dLayout() const
{
	return _d3dLayout;
}

LUCID_GAL_D3D11_END
