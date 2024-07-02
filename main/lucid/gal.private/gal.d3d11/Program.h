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
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_CORE_BEGIN

class Identity;
class Reader;

LUCID_CORE_END

LUCID_GAL_BEGIN

class Parameter;
class RenderState;

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

class Uniform;
class Sampler;
class Parameter;
class RenderState;
class VertexShader;
class GeometryShader;
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

	virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::Unordered2D const *value) const override;

	virtual void set(::lucid::gal::Parameter const *parameter, ::lucid::gal::RenderTarget2D const *value) const override;

	void onBegin() const;

	void onEnd() const;

	void onDraw() const;

private:
	::lucid::core::Identity const _identity;

	::lucid::gal::d3d11::RenderState *_renderState = nullptr;
	Sampler *_samplers = nullptr;

	VertexShader *_vertexShader = nullptr;
	GeometryShader *_geometryShader = nullptr;
	PixelShader *_pixelShader = nullptr;

	std::map<std::string, ::lucid::gal::d3d11::Parameter*> _parameters;

	void addParameterVS(Uniform const &uniform);

	void addParameterGS(Uniform const &uniform);

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

LUCID_GAL_D3D11_END