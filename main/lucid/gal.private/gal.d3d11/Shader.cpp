#include "Shader.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Error.h>

#include <fstream>
#include <d3dcompiler.h>
#include <d3d11shader.h>

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///

	Shader::Shader(std::string const &path)
	{
		try
		{
			std::ifstream stream(path.c_str(), std::ios::in | std::ios::binary);
			LUCID_VALIDATE(stream.is_open(), "unable to open: " + path);

			stream.seekg(0, std::ios::end);
			size_t size = (size_t)stream.tellg();
			stream.seekg(0, std::ios::beg);

			_code.resize(size);
			stream.read((char*)(&_code[0]), size);

			ID3D11ShaderReflection *reflector = nullptr;
			HRESULT hResult = D3DReflect(&_code[0], size, IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflector));
			GAL_VALIDATE_HRESULT(hResult, "error loading shader: " + path);

			D3D11_SHADER_DESC descShader;
			reflector->GetDesc(&descShader);

			LUCID_VALIDATE(descShader.ConstantBuffers < 2, "only one constant buffer allowed at this time");
			if (1 == descShader.ConstantBuffers)
			{
				ID3D11ShaderReflectionConstantBuffer *buffer = reflector->GetConstantBufferByIndex(0);

				D3D11_SHADER_BUFFER_DESC descBuffer;
				buffer->GetDesc(&descBuffer);

				_rawConstants.resize(descBuffer.Size);

				D3D11_BUFFER_DESC descConstants;
				::memset(&descConstants, 0, sizeof(D3D11_BUFFER_DESC));

				descConstants.Usage = D3D11_USAGE_DYNAMIC;
				descConstants.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				descConstants.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				descConstants.MiscFlags = 0;
				descConstants.ByteWidth = descBuffer.Size;

				HRESULT hResult = d3d11ConcreteDevice->CreateBuffer(&descConstants, nullptr, &_d3dConstants);
				GAL_VALIDATE_HRESULT(hResult, "unable to create shader constant buffer");

				for (uint32_t uniformIndex = 0; uniformIndex < descBuffer.Variables; ++uniformIndex)
				{
					ID3D11ShaderReflectionVariable *variable = buffer->GetVariableByIndex(uniformIndex);

					D3D11_SHADER_VARIABLE_DESC descUniform;
					variable->GetDesc(&descUniform);

					addUniform(Uniform(Uniform::TYPE_CONSTANT, descUniform.Name, descUniform.StartOffset, descUniform.Size));
				}
			}

			for (uint32_t i = 0; i < descShader.BoundResources; ++i)
			{
				D3D11_SHADER_INPUT_BIND_DESC descResource;
				reflector->GetResourceBindingDesc(i, &descResource);

				LUCID_VALIDATE(1 == descResource.BindCount, "bound resource arrays not allowed at this time");

				if (D3D_SIT_TEXTURE == descResource.Type)
				{
					addUniform(Uniform(Uniform::TYPE_TEXTURE, descResource.Name, descResource.BindPoint));
				}
				else if (D3D_SIT_SAMPLER == descResource.Type)
				{
					addUniform(Uniform(Uniform::TYPE_SAMPLER, descResource.Name, descResource.BindPoint));
				}
			}
		}
		catch (...)
		{
			throw;
		}
	}

	Shader::~Shader()
	{
		safeRelease(_d3dConstants);
	}

	void Shader::onBegin() const
	{
		///	NOP
	}

	void Shader::onEnd() const
	{
		///	NOP
	}

	void Shader::onDraw() const
	{
		if (nullptr != _d3dConstants)
		{
			D3D11_MAPPED_SUBRESOURCE mapped;

			d3d11ConcreteContext->Map(_d3dConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			::memcpy(mapped.pData, &_rawConstants[0], _rawConstants.size());
			d3d11ConcreteContext->Unmap(_d3dConstants, 0);
		}
	}

	///
	///
	///

	VertexShader::VertexShader(std::string const &path)
		: Shader(path)
	{
		std::vector<uint8_t> const &_code = code();

		HRESULT hResult = d3d11ConcreteDevice->CreateVertexShader(&_code[0], _code.size(), nullptr, &_d3dShader);
		GAL_VALIDATE_HRESULT(hResult, "unable to create shader'" + path + "'");
	}

	VertexShader::~VertexShader()
	{
		safeRelease(_d3dShader);
	}

	void VertexShader::setTexture(int32_t position, ID3D11ShaderResourceView *d3dResource)
	{
		if (-1 < position)
		{
			d3d11ConcreteContext->VSSetShaderResources(position, 1, &d3dResource);
		}
	}

	void VertexShader::setSampler(int32_t position, ID3D11SamplerState *d3dState)
	{
		if (-1 < position)
		{
			d3d11ConcreteContext->VSSetSamplers(position, 1, &d3dState);
		}
	}

	void VertexShader::onBegin() const
	{
		d3d11ConcreteContext->VSSetShader(_d3dShader, 0, 0);
		return Shader::onBegin();
	}

	void VertexShader::onEnd() const
	{
		Shader::onEnd();
		d3d11ConcreteContext->VSSetShader(0, 0, 0);
	}

	void VertexShader::onDraw() const
	{
		Shader::onDraw();

		ID3D11Buffer *_d3dConstants = d3dConstants();
		if (nullptr != _d3dConstants)
		{
			d3d11ConcreteContext->VSSetConstantBuffers(0, 1, &_d3dConstants);
		}
	}

	///
	///
	///

	PixelShader::PixelShader(std::string const &path)
		: Shader(path)
	{
		std::vector<uint8_t> const &_code = code();
		int32_t size = _code.size();

		HRESULT hResult = d3d11ConcreteDevice->CreatePixelShader(&_code[0], _code.size(), nullptr, &_d3dShader);
		GAL_VALIDATE_HRESULT(hResult, "unable to create shader '" + path + "'");
	}

	PixelShader::~PixelShader()
	{
		safeRelease(_d3dShader);
	}

	void PixelShader::setTexture(int32_t position, ID3D11ShaderResourceView *d3dResource)
	{
		if (-1 < position)
		{
			d3d11ConcreteContext->PSSetShaderResources(position, 1, &d3dResource);
		}
	}

	void PixelShader::setSampler(int32_t position, ID3D11SamplerState *d3dState)
	{
		if (-1 < position)
		{
			d3d11ConcreteContext->PSSetSamplers(position, 1, &d3dState);
		}
	}

	void PixelShader::onBegin() const
	{
		d3d11ConcreteContext->PSSetShader(_d3dShader, 0, 0);
		return Shader::onBegin();
	}

	void PixelShader::onEnd() const
	{
		Shader::onEnd();
		d3d11ConcreteContext->PSSetShader(0, 0, 0);
	}

	void PixelShader::onDraw() const
	{
		Shader::onDraw();

		ID3D11Buffer *_d3dConstants = d3dConstants();
		if (nullptr != _d3dConstants)
		{
			d3d11ConcreteContext->PSSetConstantBuffers(0, 1, &_d3dConstants);
		}
	}

}	///	d3d11
}	///	gal
}	///	lucid