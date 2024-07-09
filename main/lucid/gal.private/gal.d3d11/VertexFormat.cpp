#include "VertexFormat.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/FileReader.h>
#include <lucid/core/Reader.h>
#include <sstream>
#include <d3dcompiler.h>

LUCID_ANONYMOUS_BEGIN

/// ENUM LOOKUP
std::string const vertexElementType[] =
{
		"float",
	"float2",
	"float3",
	"float4",

		"uint",
		"uint2",
		"uint3",
		"uint4",
};

/// ENUM LOOKUP
DXGI_FORMAT const d3dFormat[] =
{
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,

	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32A32_UINT,
};

///	ENUM LOOKUP
D3D11_INPUT_CLASSIFICATION const d3dInput[] =
{
	D3D11_INPUT_PER_VERTEX_DATA,
	D3D11_INPUT_PER_INSTANCE_DATA,
};

LUCID_ANONYMOUS_END

LUCID_GAL_BEGIN

VertexFormat *VertexFormat::create(VertexElement const *src, int32_t count)
{
	std::vector<VertexElement> layout(count);
	::memcpy(&layout[0], src, count * sizeof(VertexElement));

	return create(layout);
}

VertexFormat *VertexFormat::create(std::vector<VertexElement> const &layout)
{
	return new LUCID_GAL_D3D11::VertexFormat(layout);
}

VertexFormat *VertexFormat::create(std::string const &path)
{
	return create(core::FileReader(path));
}

VertexFormat *VertexFormat::create(LUCID_CORE::Reader &reader)
{
	int32_t count = reader.read<int32_t>();

	std::vector<VertexElement> layout(count);
	reader.read(&layout[0], count * sizeof(VertexElement));

	return create(layout);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

///	createDummyCode
///
///	I do vacillate on this one.  On the one hand, I like the check between what the vertex shader
///	expects and what the vertex format describes.  On the other hand, requiring a program 
///	during the creation of a vertex format limits flexibility of implementation and hurts simplicity
///	of design.
///
///	Here now, simpliciy of design wins.  The check provided by the API is minimal at best and
///	once created, a vertex format may still be used with other shaders anyway (which renders the check
///	useless).  Also, remember, actual vertices must simply be a superset of what is described
///	by the format and the format must simply be a superset of what the shader expects.  There
///	is no check provided by the API between format and vertex stream.  Therefore, requiring vertex
///	shader code in order to create a vertex format only provides the illusion of safety.
void createDummyCode(std::vector<uint8_t> &code, std::vector<VertexElement> const &layout)
{
	std::string const header =
		"struct Input"
		"{";

	std::string const footer =
		"};"
		"struct Output"
		"{"
		"    float4 ppsPosition : POSITION0;"
		"};"
		"Output main(Input input)"
		"{"
		"    Output output = (Output)0;"
		"    return output;"
		"}";
			
	std::stringstream body;
	for (VertexElement const &element : layout)
	{
		body << "    " << vertexElementType[element.format] << " element" << element.location << " : " << "LOCATION" << element.location << ";" << std::endl;
	}

	std::string const source = header + body.str() + footer;

	ID3DBlob *blob = nullptr;
	HRESULT hResult = ::D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &blob, nullptr);
	GAL_VALIDATE_HRESULT(hResult, "unable to create 'dummy' code for vertex format");

	code.resize(blob->GetBufferSize());
	::memcpy(&code[0], blob->GetBufferPointer(), blob->GetBufferSize());

	safeRelease(blob);
}

///
///
///

VertexFormat::VertexFormat(std::vector<LUCID_GAL::VertexElement> const &layout)
{
	initialize(layout);
}

VertexFormat::~VertexFormat()
{
	shutdown();
}

void VertexFormat::initialize(std::vector<LUCID_GAL::VertexElement> const &layout)
{
	std::vector<uint8_t> code;
	LUCID_GAL_D3D11::createDummyCode(code, layout);

	_layout = layout;
	int32_t count = (int32_t)_layout.size();

	std::vector<D3D11_INPUT_ELEMENT_DESC> d3dElements(count);
	for (int32_t i = 0; i < count; ++i)
	{
		d3dElements[i].SemanticName = "LOCATION";
		d3dElements[i].SemanticIndex = _layout[i].location;

		d3dElements[i].Format = d3dFormat[_layout[i].format];
		d3dElements[i].InputSlot = _layout[i].stream;
		d3dElements[i].AlignedByteOffset = _layout[i].offset;

		d3dElements[i].InputSlotClass = d3dInput[_layout[i].type];
		d3dElements[i].InstanceDataStepRate = (_layout[i].type == VertexElement::TYPE_INSTANCE) ? 1 : 0;
	}

	HRESULT hResult = d3d11ConcreteDevice->CreateInputLayout(&d3dElements[0], count, &code[0], code.size(), &_d3dLayout);
	GAL_VALIDATE_HRESULT(hResult, "unable to create vertex format");
}

void VertexFormat::shutdown()
{
	safeRelease(_d3dLayout);
}

LUCID_GAL_D3D11_END
