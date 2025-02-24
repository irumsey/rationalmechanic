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
class Unserializer;

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
class Program : public LUCID_GAL::Program
{
public:
	Program(std::string const &path);

	Program(LUCID_CORE::Unserializer &reader);

	virtual ~Program();

	virtual LUCID_CORE::Identity const &identity() const override;

	virtual LUCID_GAL::RenderState const *renderState() const override;

	virtual LUCID_GAL::Parameter const *lookup(std::string const &name) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, uint8_t const *data, int size) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, bool value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, int32_t value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, float32_t value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Color const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Vector2 const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Vector3 const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Vector4 const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Quaternion const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Matrix2x2 const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Matrix3x3 const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Matrix4x4 const &value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Texture2D const *value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Unordered2D const *value) const override;

	virtual void set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::RenderTarget2D const *value) const override;

	void onBegin() const;

	void onEnd() const;

	void onDraw() const;

private:
	LUCID_CORE::Identity const _identity;

	LUCID_GAL_D3D11::RenderState *_renderState = nullptr;
	Sampler *_samplers = nullptr;

	VertexShader *_vertexShader = nullptr;
	GeometryShader *_geometryShader = nullptr;
	PixelShader *_pixelShader = nullptr;

	std::map<std::string, LUCID_GAL_D3D11::Parameter*> _parameters;

	void addParameterVS(Uniform const &uniform);

	void addParameterGS(Uniform const &uniform);

	void addParameterPS(Uniform const &uniform);

	void finalizeSamplers();

	void initialize(LUCID_CORE::Unserializer &reader);

	void shutdown();

	LUCID_PREVENT_COPY(Program);
	LUCID_PREVENT_ASSIGNMENT(Program);
};

inline LUCID_CORE::Identity const &Program::identity() const
{
	return _identity;
}

LUCID_GAL_D3D11_END