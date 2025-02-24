#include "Program.h"
#include "Parameter.h"
#include "Shader.h"
#include "RenderState.h"
#include "Sampler.h"
#include "Unordered2D.h"
#include "Texture2D.h"
#include "RenderTarget2D.h"
#include "Pipeline.h"
#include "System.h"
#include <lucid/core/Unserializer.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <algorithm>

LUCID_GAL_BEGIN

Program *Program::create(std::string const &path)
{
	return new LUCID_GAL_D3D11::Program(path);
}

Program *Program::create(LUCID_CORE::Unserializer &reader)
{
	return new LUCID_GAL_D3D11::Program(reader);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

Program::Program(std::string const &path)
{
	try
	{
		initialize(LUCID_CORE::Unserializer(path));
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

Program::Program(LUCID_CORE::Unserializer &reader)
{
	try
	{
		initialize(reader);
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

Program::~Program()
{
	shutdown();
}

LUCID_GAL::RenderState const *Program::renderState() const
{
	return _renderState;
}

LUCID_GAL::Parameter const *Program::lookup(std::string const &name) const
{
	auto iter = _parameters.find(name);
	if (iter != _parameters.end())
	{
		return iter->second;
	}
	return nullptr;
}

void Program::set(LUCID_GAL::Parameter const *parameter, uint8_t const *data, int size) const
{
	LUCID_GAL_D3D11::Parameter const *concrete = static_cast<LUCID_GAL_D3D11::Parameter const *>(parameter);

	if (nullptr == parameter)
	{
		return;
	}

	_vertexShader->setConstant(data, concrete->vsPosition, size);
	if (_geometryShader) { _geometryShader->setConstant(data, concrete->gsPosition, size); }
	if (_pixelShader) { _pixelShader->setConstant(data, concrete->psPosition, size); }
}

void Program::set(LUCID_GAL::Parameter const *parameter, bool value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(bool));
}

void Program::set(LUCID_GAL::Parameter const *parameter, int32_t value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(int32_t));
}

void Program::set(LUCID_GAL::Parameter const *parameter, float32_t value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(float32_t));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Color const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Color));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Vector2 const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Vector2));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Vector3 const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Vector3));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Vector4 const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Vector4));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Quaternion const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Quaternion));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Matrix2x2 const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Matrix2x2));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Matrix3x3 const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Matrix3x3));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Matrix4x4 const &value) const
{
	set(parameter, reinterpret_cast<uint8_t const *>(&value), sizeof(LUCID_GAL::Matrix4x4));
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Texture2D const *value) const
{
	LUCID_GAL_D3D11::Parameter const *concreteParameter = static_cast<LUCID_GAL_D3D11::Parameter const *>(parameter);
	LUCID_GAL_D3D11::Texture2D const *concreteTexture = static_cast<LUCID_GAL_D3D11::Texture2D const *>(value);

	if (nullptr == parameter)
	{
		return;
	}

	if (LUCID_GAL_D3D11::Uniform::TYPE_TEXTURE != concreteParameter->type)
	{
		///	TBD: error?
		return;
	}

	ID3D11ShaderResourceView * const d3dResource = concreteTexture->d3dResourceView();

	_vertexShader->setTexture(concreteParameter->vsPosition, d3dResource);
	if (_geometryShader) { _geometryShader->setTexture(concreteParameter->gsPosition, d3dResource); }
	if (_pixelShader) { _pixelShader->setTexture(concreteParameter->psPosition, d3dResource); }
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::Unordered2D const *value) const
{
	LUCID_GAL_D3D11::Parameter const *concreteParameter = static_cast<LUCID_GAL_D3D11::Parameter const *>(parameter);
	LUCID_GAL_D3D11::Unordered2D const *concreteTexture = static_cast<LUCID_GAL_D3D11::Unordered2D const *>(value);

	if (nullptr == parameter)
	{
		return;
	}

	if (LUCID_GAL_D3D11::Uniform::TYPE_TEXTURE != concreteParameter->type)
	{
		///	TBD: error?
		return;
	}

	ID3D11ShaderResourceView * const d3dResource = concreteTexture->d3dResourceView();

	_vertexShader->setTexture(concreteParameter->vsPosition, d3dResource);
	if (_geometryShader) { _geometryShader->setTexture(concreteParameter->gsPosition, d3dResource); }
	if (_pixelShader) { _pixelShader->setTexture(concreteParameter->psPosition, d3dResource); }
}

void Program::set(LUCID_GAL::Parameter const *parameter, LUCID_GAL::RenderTarget2D const *value) const
{
	LUCID_GAL_D3D11::Parameter const *concreteParameter = static_cast<LUCID_GAL_D3D11::Parameter const *>(parameter);
	LUCID_GAL_D3D11::RenderTarget2D const *concreteTarget = static_cast<LUCID_GAL_D3D11::RenderTarget2D const *>(value);

	if (nullptr == parameter)
	{
		return;
	}

	if (LUCID_GAL_D3D11::Uniform::TYPE_TEXTURE != concreteParameter->type)
	{
		///	TBD: error?
		return;
	}

	ID3D11ShaderResourceView * const d3dResource = concreteTarget->d3dResourceView();

	_vertexShader->setTexture(concreteParameter->vsPosition, d3dResource);
	if (_geometryShader) { _geometryShader->setTexture(concreteParameter->gsPosition, d3dResource); }
	if (_pixelShader) { _pixelShader->setTexture(concreteParameter->psPosition, d3dResource); }
}

void Program::onBegin() const
{
	for (Sampler const *sampler = _samplers; sampler; sampler = sampler->next)
	{
		SamplerState const &state = sampler->state;

		_vertexShader->setSampler(sampler->vsPosition, state.d3dState);
		if (_geometryShader) { _geometryShader->setSampler(sampler->gsPosition, state.d3dState); }
		if (_pixelShader) { _pixelShader->setSampler(sampler->psPosition, state.d3dState); }
	}

	d3d11ConcreteContext->RSSetState(_renderState->d3dRasterizerState());
	d3d11ConcreteContext->OMSetDepthStencilState(_renderState->d3dDepthStencilState(), 0xFF);
	d3d11ConcreteContext->OMSetBlendState(_renderState->d3dBlendState(), nullptr, 0xFFFFFFFF);

	_vertexShader->onBegin();
	if (_geometryShader) { _geometryShader->onBegin(); }
	if (_pixelShader) { _pixelShader->onBegin(); }
}

void Program::onEnd() const
{
	_vertexShader->onEnd();
	if (_geometryShader) { _geometryShader->onEnd(); }
	if (_pixelShader) { _pixelShader->onEnd(); }
}

void Program::onDraw() const
{
	_vertexShader->onDraw();
	if (_geometryShader) { _geometryShader->onDraw(); }
	if (_pixelShader) { _pixelShader->onDraw(); }
}

void Program::addParameterVS(Uniform const &uniform)
{
	auto iter = _parameters.find(uniform.name);
	if (iter == _parameters.end())
	{
		Parameter *parameter = new Parameter(uniform.type, uniform.name);

		parameter->vsPosition = uniform.position;
		parameter->size = uniform.size;

		_parameters[uniform.name] = parameter;
	}
	else
	{
		Parameter *parameter = iter->second;

		LUCID_VALIDATE(uniform.type == parameter->type, "vertex shader type mismatch: " + uniform.name);
		LUCID_VALIDATE(uniform.size == parameter->size, "vertex shader type mismatch: " + uniform.name);

		parameter->vsPosition = uniform.position;
	}
}

void Program::addParameterGS(Uniform const &uniform)
{
	auto iter = _parameters.find(uniform.name);
	if (iter == _parameters.end())
	{
		Parameter *parameter = new Parameter(uniform.type, uniform.name);

		parameter->gsPosition = uniform.position;
		parameter->size = uniform.size;

		_parameters[uniform.name] = parameter;
	}
	else
	{
		Parameter *parameter = iter->second;

		LUCID_VALIDATE(uniform.type == parameter->type, "geometry shader type mismatch: " + uniform.name);
		LUCID_VALIDATE(uniform.size == parameter->size, "geometry shader type mismatch: " + uniform.name);

		parameter->gsPosition = uniform.position;
	}
}

void Program::addParameterPS(Uniform const &uniform)
{
	auto iter = _parameters.find(uniform.name);
	if (iter == _parameters.end())
	{
		Parameter *parameter = new Parameter(uniform.type, uniform.name);

		parameter->psPosition = uniform.position;
		parameter->size = uniform.size;

		_parameters[uniform.name] = parameter;
	}
	else
	{
		Parameter *parameter = iter->second;

		LUCID_VALIDATE(uniform.type == parameter->type, "pixel shader type mismatch: " + uniform.name);
		LUCID_VALIDATE(uniform.size == parameter->size, "pixel shader type mismatch: " + uniform.name);

		parameter->psPosition = uniform.position;
	}
}

void Program::finalizeSamplers()
{
	for (Sampler *sampler = _samplers; sampler; sampler = sampler->next)
	{
		auto iter = _parameters.find(sampler->name);
		if (iter != _parameters.end())
		{
			Parameter const *parameter = iter->second;

			sampler->vsPosition = parameter->vsPosition;
			sampler->gsPosition = parameter->gsPosition;
			sampler->psPosition = parameter->psPosition;
		}
	}
}

void Program::initialize(LUCID_CORE::Unserializer &reader)
{
	try
	{
		int32_t count = reader.read<int32_t>();
		for (int32_t i = 0; i < count; ++i)
		{
			reader.member_begin();

			std::string name = reader.read<std::string>();
			Sampler *sampler = new Sampler(name, reader);

			sampler->next = _samplers;
			_samplers = sampler;

			reader.member_end();
		}

		reader.member_begin();
		_renderState = new RenderState(reader);
		reader.member_end();

		_vertexShader = new VertexShader(reader.read<std::string>());
		for (auto uniform : _vertexShader->uniforms()) { addParameterVS(uniform); }

		std::string geometryShaderPath = reader.read<std::string>();
		if (0 != geometryShaderPath.size())
		{
			_geometryShader = new GeometryShader(geometryShaderPath);
			for (auto uniform : _geometryShader->uniforms()) { addParameterGS(uniform); }
		}

		std::string pixelShaderPath = reader.read<std::string>();
		if (0 != pixelShaderPath.size())
		{
			_pixelShader = new PixelShader(pixelShaderPath);
			for (auto uniform : _pixelShader->uniforms()) { addParameterPS(uniform); }
		}

		finalizeSamplers();
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}

	++galConcreteStatistic(programs);
}

void Program::shutdown()
{
	delete _pixelShader;
	delete _geometryShader;
	delete _vertexShader;

	delete _samplers;

	delete _renderState;

	for (auto iter : _parameters) { delete iter.second; }

	--galConcreteStatistic(programs);
}

LUCID_GAL_D3D11_END
