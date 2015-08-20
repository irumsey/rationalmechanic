#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/VertexFormat.h>

///
///
///
namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///
	class VertexFormat : public ::lucid::gal::VertexFormat
	{
	public:
		VertexFormat(std::vector<::lucid::gal::VertexElement> const &layout);

		virtual ~VertexFormat();

		virtual std::vector<::lucid::gal::VertexElement> const &layout() const override;

		ID3D11InputLayout *d3dLayout() const;

	private:
		std::vector<::lucid::gal::VertexElement> _layout;
		ID3D11InputLayout *_d3dLayout = nullptr;

		void initialize(std::vector<::lucid::gal::VertexElement> const &layout);

		void shutdown();

		LUCID_PREVENT_COPY(VertexFormat);
		LUCID_PREVENT_ASSIGNMENT(VertexFormat);
	};

	inline std::vector<::lucid::gal::VertexElement> const &VertexFormat::layout() const
	{
		return _layout;
	}

	inline ID3D11InputLayout *VertexFormat::d3dLayout() const
	{
		return _d3dLayout;
	}

}	///	d3d11
}	///	gal
}	///	lucid