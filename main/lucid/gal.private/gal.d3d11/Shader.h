#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <cassert>
#include <cstdint>
#include <vector>
#include <string>
#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_GAL_D3D11_BEGIN

///
///
///
class Uniform
{
public:
	enum TYPE
	{
		TYPE_UNDEFINED = -1,

		TYPE_CONSTANT = 0,
		TYPE_TEXTURE = 1,
		TYPE_SAMPLER = 2,

		TYPE_TEXTURE_RANDOM_ACCESS = 3,
	};

	TYPE type = TYPE_UNDEFINED;
	std::string name = "<undefined>";
	int32_t position = -1;
	int32_t size = 0;

	Uniform() = default;

	Uniform(TYPE type, std::string const &name, int32_t position, int32_t size = 0)
		: type(type)
		, name(name)
		, position(position)
		, size(size)
	{
	}
};

///
///
///
class Shader
{
public:
	virtual ~Shader();

	std::vector<uint8_t> const &code() const;

	std::vector<Uniform> const &uniforms() const;

	void addUniform(Uniform const &uniform);

	void setConstant(uint8_t const *data, int32_t position, int32_t size);

	virtual void setTexture(int32_t position, ID3D11ShaderResourceView *d3dResource) = 0;

	virtual void setSampler(int32_t position, ID3D11SamplerState *d3dState) = 0;

	virtual void onBegin() const;

	virtual void onEnd() const;

	virtual void onDraw() const;

	ID3D11Buffer *d3dConstants() const;

protected:
	Shader(std::string const &path);

private:
	std::vector<uint8_t> _code;
	std::vector<Uniform> _uniforms;

	std::vector<uint8_t> _rawConstants;
	ID3D11Buffer *_d3dConstants = nullptr;

	LUCID_PREVENT_COPY(Shader);
	LUCID_PREVENT_ASSIGNMENT(Shader);
};

inline std::vector<uint8_t> const &Shader::code() const
{
	return _code;
}

inline std::vector<Uniform> const &Shader::uniforms() const
{
	return _uniforms;
}

inline void Shader::setConstant(uint8_t const *data, int32_t position, int32_t size)
{
	if (-1 < position)
	{
		assert(0 != _rawConstants.size());
		::memcpy(&_rawConstants[position], data, size);
	}
}

inline void Shader::addUniform(Uniform const &uniform)
{
	_uniforms.push_back(uniform);
}

inline ID3D11Buffer *Shader::d3dConstants() const
{
	return _d3dConstants;
}

///
///
///
class VertexShader : public Shader
{
public:
	VertexShader(std::string const &path);

	virtual ~VertexShader();

	virtual void setTexture(int32_t position, ID3D11ShaderResourceView *d3dResource) override;

	virtual void setSampler(int32_t position, ID3D11SamplerState *d3dState) override;

	virtual void onBegin() const override;

	virtual void onEnd() const override;

	virtual void onDraw() const override;

	ID3D11VertexShader *d3dShader() const;

private:
	ID3D11VertexShader *_d3dShader = nullptr;

	LUCID_PREVENT_COPY(VertexShader);
	LUCID_PREVENT_ASSIGNMENT(VertexShader);
};

inline ID3D11VertexShader *VertexShader::d3dShader() const
{
	return _d3dShader;
}

///
///
///
class GeometryShader : public Shader
{
public:
	GeometryShader(std::string const &path);

	virtual ~GeometryShader();

	virtual void setTexture(int32_t position, ID3D11ShaderResourceView *d3dResource) override;

	virtual void setSampler(int32_t position, ID3D11SamplerState *d3dState) override;

	virtual void onBegin() const override;

	virtual void onEnd() const override;

	virtual void onDraw() const override;

	ID3D11GeometryShader *d3dShader() const;

private:
	ID3D11GeometryShader *_d3dShader = nullptr;

	LUCID_PREVENT_COPY(GeometryShader);
	LUCID_PREVENT_ASSIGNMENT(GeometryShader);
};

inline ID3D11GeometryShader *GeometryShader::d3dShader() const
{
	return _d3dShader;
}

///
///
///
class PixelShader : public Shader
{
public:
	PixelShader(std::string const &path);

	virtual ~PixelShader();

	virtual void setTexture(int32_t position, ID3D11ShaderResourceView *d3dResource) override;

	virtual void setSampler(int32_t position, ID3D11SamplerState *d3dState) override;

	virtual void onBegin() const override;

	virtual void onEnd() const override;

	virtual void onDraw() const override;

	ID3D11PixelShader *d3dShader() const;

private:
	ID3D11PixelShader *_d3dShader = nullptr;

	LUCID_PREVENT_COPY(PixelShader);
	LUCID_PREVENT_ASSIGNMENT(PixelShader);
};

inline ID3D11PixelShader *PixelShader::d3dShader() const
{
	return _d3dShader;
}

LUCID_GAL_D3D11_END
