#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <string>
#include <vector>
#include <map>
#include <lucid/core/Types.h>
#include <lucid/core/Identity.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/Matrix.h>
#include <lucid/gal/Types.h>
#include <lucid/gal/Program.h>

///
///
///
namespace lucid {
namespace core {

	class Identity;
	class Reader;

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace gal {

	class Parameter;
	class RenderState;

}	///	gal
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

	class Uniform;
	class Sampler;
	class Parameter;
	class RenderState;
	class VertexShader;
	class PixelShader;

	///
	///
	///
	class Program : public ::lucid::gal::Program
	{
	public:
		Program(std::string const &path);

		Program(::lucid::core::Reader &reader);

		virtual ~Program();

		virtual ::lucid::core::Identity const &identity() const override;

		virtual ::lucid::gal::RenderState const *renderState() const override;

		virtual ::lucid::gal::Parameter const *lookup(std::string const &name) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, uint8_t const *data, int size) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, bool value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, int32_t value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, float32_t value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Color const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Vector2 const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Vector3 const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Vector4 const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Quaternion const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Matrix2x2 const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Matrix3x3 const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Matrix4x4 const &value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Texture2D const *value) const override;

		virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::RenderTarget2D const *value) const override;

		void onBegin() const;

		void onEnd() const;

		void onDraw() const;

		std::vector<uint8_t> const &vertexCode() const;

		std::vector<uint8_t> const &pixelCode() const;

	private:
		::lucid::core::Identity const _identity;

		::lucid::gal::d3d11::RenderState *_renderState = nullptr;
		Sampler *_samplers = nullptr;

		VertexShader *_vertexShader = nullptr;
		PixelShader *_pixelShader = nullptr;

		std::map<std::string, ::lucid::gal::d3d11::Parameter*> _parameters;

		void addParameterVS(Uniform const &uniform);

		void addParameterPS(Uniform const &uniform);

		void finalizeSamplers();

		void initialize(::lucid::core::Reader &reader);

		void shutdown();

		LUCID_PREVENT_COPY(Program);
		LUCID_PREVENT_ASSIGNMENT(Program);
	};

	inline ::lucid::core::Identity const &Program::identity() const
	{
		return _identity;
	}

}	///	d3d11
}	///	gal
}	///	lucid
