﻿#
#	imports
#

import os
import optparse
import uuid
import json
import struct

#
#	enums
#

#	render target format
renderTargetFormat = {
	  'FORMAT_UNORM_R8G8B8A8' :  0,
	     'FORMAT_UINT_R16G16' :  1,
	'FORMAT_UINT_R10G10B10A2' :  2,
	    'FORMAT_FLOAT_R16G16' :  3,
		   'FORMAT_FLOAT_R32' :  4,
	    'FORMAT_FLOAT_R32G32' :  5,
            'FORMAT_UINT_R32' :  6,
}

#	depth target format
depthTargetFormat = {
	  'FORMAT_UINT_D16' :  0,
	'FORMAT_UINT_D24S8' :  1,
	 'FORMAT_FLOAT_D32' :  2,
}

#	vertex element format
vertexElementFormat = {
	   'FORMAT_FLOAT1' :  0,
	   'FORMAT_FLOAT2' :  1,
	   'FORMAT_FLOAT3' :  2,
	   'FORMAT_FLOAT4' :  3,
	    'FORMAT_UINT1' :  4,
	    'FORMAT_UINT2' :  5,
	    'FORMAT_UINT3' :  6,
	    'FORMAT_UINT4' :  7,
}

#	vertex element type
vertexElementType = {
	   'TYPE_VERTEX' :  0,
	 'TYPE_INSTANCE' :  1,
}

#	buffer usage
bufferUsage = {
	   'USAGE_STATIC' :  0,
	  'USAGE_DYNAMIC' :  1,
}

#	index buffer format
indexBufferFormat = {
	'FORMAT_UINT16' : 0,
	'FORMAT_UINT32' : 1,
}

#	filter state
filterState = {
	                         'FILTER_MIN_MAG_MIP_POINT' :  0,
	                  'FILTER_MIN_MAG_POINT_MIP_LINEAR' :  1,
	            'FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT' :  2,
	                  'FILTER_MIN_POINT_MAG_MIP_LINEAR' :  3,
	                  'FILTER_MIN_LINEAR_MAG_MIP_POINT' :  4,
	           'FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR' :  5,
	                  'FILTER_MIN_MAG_LINEAR_MIP_POINT' :  6,
	                        'FILTER_MIN_MAG_MIP_LINEAR' :  7,
	                               'FILTER_ANISOTROPIC' :  8,
	              'FILTER_COMPARISON_MIN_MAG_MIP_POINT' :  9,
	       'FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR' : 10,
	 'FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT' : 11,
	       'FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR' : 12,
	       'FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT' : 13,
	'FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR' : 14,
	       'FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT' : 15,
	             'FILTER_COMPARISON_MIN_MAG_MIP_LINEAR' : 16,
	                    'FILTER_COMPARISON_ANISOTROPIC' : 17,
	                 'FILTER_MINIMUM_MIN_MAG_MIP_POINT' : 18,
	          'FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR' : 19,
	    'FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT' : 20,
	          'FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR' : 21,
	          'FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT' : 22,
	   'FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR' : 23,
	          'FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT' : 24,
	                'FILTER_MINIMUM_MIN_MAG_MIP_LINEAR' : 25,
	                       'FILTER_MINIMUM_ANISOTROPIC' : 26,
	                 'FILTER_MAXIMUM_MIN_MAG_MIP_POINT' : 27,
	          'FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR' : 28,
	    'FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT' : 29,
	          'FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR' : 30,
	          'FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT' : 31,
	   'FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR' : 32,
	          'FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT' : 33,
	                'FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR' : 34,
	                       'FILTER_MAXIMUM_ANISOTROPIC' : 35,
}

#	address mode
addressMode = {
	       'ADDRESS_WRAP' : 0,
	     'ADDRESS_MIRROR' : 1,
	      'ADDRESS_CLAMP' : 2,
	     'ADDRESS_BORDER' : 3,
	'ADDRESS_MIRROR_ONCE' : 4,
}

#	compraison
comparison = {
	        'COMPARISON_NEVER' : 0,
	         'COMPARISON_LESS' : 1,
	        'COMPARISON_EQUAL' : 2,
	   'COMPARISON_LESS_EQUAL' : 3,
	      'COMPARISON_GREATER' : 4,
	    'COMPARISON_NOT_EQUAL' : 5,
	'COMPARISON_GREATER_EQUAL' : 6,
	       'COMPARISON_ALWAYS' : 7,
}

#	fill mode
fillMode = {
	    'FILL_SOLID' : 0,
	'FILL_WIREFRAME' : 1,
}

#	cullMode
cullMode = {
	'CULL_FRONT' : 0,
	 'CULL_BACK' : 1,
	 'CULL_NONE' : 2,
}

#	
depthWriteMask = {
	'DEPTH_WRITE_DISABLED' : 0,
	 'DEPTH_WRITE_ENABLED' : 1,
}

#
stencilOperation = {
	    'STENCIL_KEEP' : 0,
	    'STENCIL_ZERO' : 1,
	 'STENCIL_REPLACE' : 2,
	'STENCIL_INCR_SAT' : 3,
	    'STENCIL_INCR' : 4,
	'STENCIL_DECR_SAT' : 5,
	    'STENCIL_DECR' : 6,
	  'STENCIL_INVERT' : 7,
}

#
blendMode = {
	           'BLEND_ONE' : 0,
	     'BLEND_SRC_COLOR' : 1,
	 'BLEND_INV_SRC_COLOR' : 2,
	    'BLEND_DEST_COLOR' : 3,
	'BLEND_INV_DEST_COLOR' : 4,
	     'BLEND_SRC_ALPHA' : 5,
	 'BLEND_INV_SRC_ALPHA' : 6,
	    'BLEND_DEST_ALPHA' : 7,
	'BLEND_INV_DEST_ALPHA' : 8,
}

#
topology = {
	        'TOPOLOGY_POINT_LIST' : 0,
	         'TOPOLOGY_LINE_LIST' : 1,
	        'TOPOLOGY_LINE_STRIP' : 2,
	     'TOPOLOGY_TRIANGLE_LIST' : 3,
	    'TOPOLOGY_TRIANGLE_STRIP' : 4,
	'TOPOLOGY_TRIANGLE_ADJACENCY' : 5,
}

#
#	conversions
#

def deg2rad(deg):
	return 0.01745329251994329576923690768489 * deg

def au2meter(au):
	return 149597870700.0 * au

#
#	bootstrap basic types
#

def bootIdentity(dst, value):
	uid = uuid.UUID(value)
	for i in range(16):
		dst.write(struct.pack('B', uid.bytes[i]))

def bootString(dst, value):
	l = len(value)
	dst.write(struct.pack('i', l))
	dst.write(struct.pack(str(l) + 's', bytes(value, 'ascii')))

def bootBoolean(dst, value):
	dst.write(struct.pack('?', value))

def bootIndex(dst, value):
	dst.write(struct.pack('H', value))

def bootByte(dst, value):
	dst.write(struct.pack('B', value))

def bootInteger(dst, value):
	dst.write(struct.pack('i', value))

def bootUnsigned(dst, value):
	dst.write(struct.pack('I', value))

def bootFloat(dst, value):
	dst.write(struct.pack('f', value))

def bootDouble(dst, value):
	dst.write(struct.pack('d', value))

def bootDegreesAsRadians(dst, value):
	bootDouble(dst, deg2rad(value))

def bootDegreesAsRadians3(dst, values):
	bootDegreesAsRadians(dst, values[0])
	bootDegreesAsRadians(dst, values[1])
	bootDegreesAsRadians(dst, values[2])
	
def bootAUsAsMeters(dst, value):
	bootDouble(dst, au2meter(value))

def bootVector2(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))

def bootVector3(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))
	dst.write(struct.pack('f', value[2]))
	
def bootVector4(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))
	dst.write(struct.pack('f', value[2]))
	dst.write(struct.pack('f', value[3]))

def bootQuaternion(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))
	dst.write(struct.pack('f', value[2]))
	dst.write(struct.pack('f', value[3]))

def bootMatrix2x2(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))

	dst.write(struct.pack('f', value[2]))
	dst.write(struct.pack('f', value[3]))

def bootMatrix3x3(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))
	dst.write(struct.pack('f', value[2]))

	dst.write(struct.pack('f', value[3]))
	dst.write(struct.pack('f', value[4]))
	dst.write(struct.pack('f', value[5]))

	dst.write(struct.pack('f', value[6]))
	dst.write(struct.pack('f', value[7]))
	dst.write(struct.pack('f', value[8]))

def bootMatrix4x4(dst, value):
	dst.write(struct.pack('f', value[ 0]))
	dst.write(struct.pack('f', value[ 1]))
	dst.write(struct.pack('f', value[ 2]))
	dst.write(struct.pack('f', value[ 3]))

	dst.write(struct.pack('f', value[ 4]))
	dst.write(struct.pack('f', value[ 5]))
	dst.write(struct.pack('f', value[ 6]))
	dst.write(struct.pack('f', value[ 7]))

	dst.write(struct.pack('f', value[ 8]))
	dst.write(struct.pack('f', value[ 9]))
	dst.write(struct.pack('f', value[10]))
	dst.write(struct.pack('f', value[11]))

	dst.write(struct.pack('f', value[12]))
	dst.write(struct.pack('f', value[13]))
	dst.write(struct.pack('f', value[14]))
	dst.write(struct.pack('f', value[15]))

def bootColor(dst, value):
	dst.write(struct.pack('f', value[0]))
	dst.write(struct.pack('f', value[1]))
	dst.write(struct.pack('f', value[2]))
	dst.write(struct.pack('f', value[3]))

def bootTexture2D(dst, value):
	bootString(dst, value)

def bootTarget2D(dst, value):
	dst.write(struct.pack('i', renderTargetFormat[value['format']]))
	dst.write(struct.pack('i', value['width']))
	dst.write(struct.pack('i', value['height']))

def bootDepthBuffer(dst, value):
	dst.write(struct.pack('i', depthTargetFormats[value['format']]))
	dst.write(struct.pack('i', value['width']))
	dst.write(struct.pack('i', value['height']))

#
#
#
primitiveLookup = [
	[       'boolean',        bootBoolean,  2],
	[       'integer',        bootInteger,  3],
	[         'float',          bootFloat,  4],
	[         'color',          bootColor,  5],
	[       'vector2',        bootVector2,  6],
	[       'vector3',        bootVector3,  7],
	[       'vector4',        bootVector4,  8],
	[    'quaternion',     bootQuaternion,  9],
	[     'matrix2x2',      bootMatrix2x2, 10],
	[     'matrix3x3',      bootMatrix3x3, 11],
	[     'matrix4x4',      bootMatrix4x4, 12],
	[     'texture2d',      bootTexture2D, 13],
	[      'target2d',       bootTarget2D, 14],
	[         'query',         bootString, 15],
]

#
#	bootstrap texture samplers
#

def bootSampler(dst, sampler):
	bootUnsigned(dst, filterState[sampler['Filter']])

	bootUnsigned(dst, addressMode[sampler['AddressU']])
	bootUnsigned(dst, addressMode[sampler['AddressV']])
	bootUnsigned(dst, addressMode[sampler['AddressW']])

	bootFloat(dst, sampler['lodMipBias'])
	bootUnsigned(dst, sampler['MaxAnisotropy'])

	bootUnsigned(dst, comparison[sampler['Compare']])
	
	bootColor(dst, sampler['BorderColor'])

	bootFloat(dst, sampler['MinLOD'])
	bootFloat(dst, sampler['MaxLOD'])

def bootSamplers(dst, samplers):
	bootInteger(dst, len(samplers))

	for sampler in samplers:
		bootString(dst, sampler['name'])
		bootSampler(dst, sampler)

def bootSamplersFromFile(srcPath, dstPath):
	bootSamplers(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#	bootstrap render states
#

def bootRenderStates(dst, states):
	bootUnsigned(dst, fillMode[states['RasterMode']['Fill']])
	bootUnsigned(dst, cullMode[states['RasterMode']['Cull']])
	bootBoolean(dst, states['RasterMode']['Multisample'])

	bootByte(dst, states['BlendMode']['ColorWriteMask'])
	bootBoolean(dst, states['BlendMode']['Enabled'])
	bootUnsigned(dst, blendMode[states['BlendMode']['SrcBlend']])
	bootUnsigned(dst, blendMode[states['BlendMode']['SrcBlendAlpha']])
	bootUnsigned(dst, blendMode[states['BlendMode']['DstBlend']])
	bootUnsigned(dst, blendMode[states['BlendMode']['DstBlendAlpha']])

	bootBoolean(dst, states['DepthStencilMode']['DepthEnabled'])
	bootUnsigned(dst, depthWriteMask[states['DepthStencilMode']['DepthWrite']])
	bootInteger(dst, comparison[states['DepthStencilMode']['DepthFunction']])

	bootBoolean(dst, states['DepthStencilMode']['StencilEnabled'])
	bootInteger(dst, comparison[states['DepthStencilMode']['StencilFunction']])

	bootInteger(dst, stencilOperation[states['DepthStencilMode']['StencilPass']])
	bootInteger(dst, stencilOperation[states['DepthStencilMode']['StencilFail']])
	bootInteger(dst, stencilOperation[states['DepthStencilMode']['StencilDepthFail']])

	bootByte(dst, states['DepthStencilMode']['StencilReadMask'])
	bootByte(dst, states['DepthStencilMode']['StencilWriteMask'])

	bootByte(dst, states['DepthStencilMode']['StencilRef'])

def bootRenderStatesFromFile(srcPath, dstPath):
	bootRenderStates(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#	bootstrap program
#

def bootProgram(dst, program):
	bootSamplers(dst, program['Samplers'])
	bootRenderStates(dst, program['RenderStates'])
	
	bootString(dst, program['VertexShader']['Path'])

	if 'GeometryShader' in program:
		bootBoolean(dst, True)
		bootString(dst, program['GeometryShader']['Path'])
	else:
		bootBoolean(dst, False)

	if 'PixelShader' in program:
		bootBoolean(dst, True)
		bootString(dst, program['PixelShader']['Path'])
	else:
		bootBoolean(dst, False)

def bootProgramFromFile(srcPath, dstPath):
	bootProgram(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#	bootstrap material
#

def bootMaterialAttribute(dst, attribute):
	name = attribute['name']
	bootString(dst, name)

	found = False
	for type in primitiveLookup:
		if type[0] in attribute:
			dst.write(struct.pack('i', type[2]))
			type[1](dst, attribute[type[0]])
			found = True

	if not found:
		raise Exception('unknown material attribute type')

def bootMaterialAttributes(dst, attributes):
	dst.write(struct.pack('i', len(attributes)))

	for attribute in attributes:
		bootMaterialAttribute(dst, attribute)

def bootMaterial(dst, material):
	isReference = isinstance(material['program'], str)
	bootBoolean(dst, isReference)

	if isReference:
		bootString(dst, material['program'])
	else:
		bootProgram(dst, material['program'])

	bootMaterialAttributes(dst, material['attributes'])

def bootMaterialFromFile(srcPath, dstPath):
	bootMaterial(open(dstPath, 'wb'), json.load(open(srcPath)))


#	bootstrp VertexFormat
#
#
def bootVertexFormat(dst, src):
	elements = src['elements']

	dst.write(struct.pack('i', len(elements)))
	for element in elements:
		dst.write(struct.pack('i', vertexElementFormat[element['format']]))
		dst.write(struct.pack('i', vertexElementType[element['type']]))
		dst.write(struct.pack('i', element['location']))
		dst.write(struct.pack('i', element['stream']))
		dst.write(struct.pack('i', 4 * element['offset']))

def bootVertexFormatFromFile(srcPath, dstPath):
	bootVertexFormat(open(dstPath, 'wb'), json.load(open(srcPath)))

#	bootstrap VertexBuffer
#
#
def bootVertexBuffer(dst, src):
	data = src['data']
	stride = src['stride']

	bootInteger(dst, bufferUsage[src['usage']])
	bootInteger(dst, (int)(len(data) / stride))
	bootInteger(dst, 4 * stride)

	for i in range(len(data)):
		bootFloat(dst, data[i])

def bootVertexBufferFromFile(srcPath, dstPath):
	bootVertexBuffer(open(dstPath, 'wb'), json.load(open(srcPath)))

#	bootstrap IndexBuffer
#
#
def bootIndexBuffer(dst, src):
	data = src['data']

	uint16 = ('FORMAT_UINT16' == src['format'])

	bootInteger(dst, bufferUsage[src['usage']])
	bootInteger(dst, indexBufferFormat[src['format']])

	bootInteger(dst, len(data))

	for i in range(len(data)):
		if uint16:
			bootIndex(dst, data[i])
		else:
			bootUnsigned(dst, data[i])

def bootIndexBufferFromFile(srcPath, dstPath):
	bootIndexBuffer(open(dstPath, 'wb'), json.load(open(srcPath)))

#	bootstrap Geometry
#
#
def bootGeometry(dst, geometry):
	bootInteger(dst, topology[geometry['topology']])
		
	isReference = isinstance(geometry['format'], str)
	bootBoolean(dst, isReference)
	if isReference:		
		bootString(dst, geometry['format'])
	else:
		bootVertexFormat(dst, geometry['format'])
			
	bootVertexBuffer(dst, geometry['vertices'])
	bootIndexBuffer(dst, geometry['indices'])
			
def bootGeometryFromFile(srcPath, dstPath):
	bootGeometry(open(dstPath, 'wb'), json.load(open(srcPath)))

#	bootstrap Mesh
#
#
def bootMesh(dst, src):
	isReference = isinstance(src['material'], str)
	bootBoolean(dst, isReference)
	if isReference:
		bootString(dst, src['material'])
	else:
		bootMaterial(dst, src['material'])

	isReference = isinstance(src['geometry'], str)
	bootBoolean(dst, isReference)
	if isReference:
		bootString(dst, src['geometry'])	
	else:
		bootGeometry(dst, src['geometry'])

def bootMeshFromFile(srcPath, dstPath):
	bootMesh(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#	bootstrap Font
#	
def bootFont(dst, src):
	bootString(dst, src['name'])
	bootFloat(dst, src['default spacing'])
	bootString(dst, src['mesh'])
	glyphs = src['glyphs']
	bootInteger(dst, len(glyphs))
	for glyph in glyphs:
		bootByte(dst, glyph['code'].encode()[0])
		bootVector2(dst, glyph['texcoord'])
		bootVector2(dst, glyph['texsize'])
		bootColor(dst, glyph['channel'])
	spacing = src['spacing']
	bootInteger(dst, len(spacing))
	# TBD: implement...
		
def bootFontFromFile(srcPath, dstPath):
	bootFont(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#	bootstrap render context
#

def bootContextPrimitive(dst, primitive):
	bootString(dst, primitive['name'])

	found = False
	for type in primitiveLookup:
		if type[0] in primitive:
			dst.write(struct.pack('i', type[2]))
			type[1](dst, primitive[type[0]])
			found = True

	if not found:
		raise Exception('unknown context primitive type')

def bootContext(dst, context):
	primitives = context['primitives']
	bootInteger(dst, len(primitives))
	for primitive in primitives:
		bootContextPrimitive(dst, primitive)

def bootContextFromFile(srcPath, dstPath):
	bootContext(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#
#
def bootCamera(dst, camera):
	type = camera['type']

	if 'perspective' == type:
		bootUnsigned(dst, 0)
		bootFloat(dst, camera['fov'])
		bootFloat(dst, camera['aspect'])
		bootFloat(dst, camera['znear'])
		bootFloat(dst, camera['zfar'])
	else:
		bootUnsigned(dst, 1)
		bootFloat(dst, camera['width'])
		bootFloat(dst, camera['height'])
		bootFloat(dst, camera['znear'])
		bootFloat(dst, camera['zfar'])

	bootVector3(dst, camera['position'])
	bootVector3(dst, camera['target'])
	bootVector3(dst, camera['up'])

def bootCameraFromFile(srcPath, dstPath):
	bootCamera(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#
#

def bootStarCatalog(dst, catalog):
	stars = catalog['catalog']
	bootInteger(dst, len(stars))
	for star in stars:
		bootInteger(dst, star['XNO'])
		bootString(dst, star['TYPE'])
		bootDouble(dst, star['RA'])
		bootDouble(dst, star['DEC'])
		bootFloat(dst, star['MAG'])

def bootStarCatalogFromFile(srcPath, dstPath):
	bootStarCatalog(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#
#

orbitalFrameType = {
	'DYNAMIC_POINT' : 1,
	'ORBITAL_BODY'  : 2,
	'DYNAMIC_BODY'  : 3,
	'CAMERA_FRAME'  : 4,
}

onOrbitalElement = [
	bootDouble,
	bootDouble,
	bootAUsAsMeters,
	bootDegreesAsRadians,
	bootDegreesAsRadians,
	bootDegreesAsRadians,
	bootDouble,
	bootDegreesAsRadians,
	bootDegreesAsRadians,
	bootDegreesAsRadians,
	bootAUsAsMeters,
	bootAUsAsMeters,
	bootDouble
]

def bootFrameDynamicPoint(dst, frame):
	bootUnsigned(dst, orbitalFrameType['DYNAMIC_POINT'])
	bootUnsigned(dst, frame['hid'])
	bootString(dst, frame['target'])
	bootString(dst, frame['description'])
	bootString(dst, frame['center'])

def bootFrameOrbitalBody(dst, frame):
	bootUnsigned(dst, orbitalFrameType['ORBITAL_BODY'])
	bootInteger(dst, frame['hid'])
	bootString(dst, frame['target'])
	bootString(dst, frame['description'])
	bootString(dst, frame['center'])

	properties = frame['properties']

	bootBoolean(dst, properties['render']['show orbit'])
	bootBoolean(dst, properties['render']['show icon'])

	model = properties['render']['model']
	bootUnsigned(dst, len(model))
	for mesh in model:
		bootString(dst, mesh)			
	
	bootString(dst, properties['render']['icon']['mesh'])
	bootVector4(dst, properties['render']['icon']['parameters'])
	
	bootVector4(dst, properties['render']['channel0'])
	bootVector4(dst, properties['render']['channel1'])
	bootVector4(dst, properties['render']['channel2'])

	bootDouble(dst, properties['physical']['GM'])
	bootDouble(dst, properties['physical']['mass'])
	bootDouble(dst, properties['physical']['radius'])
	
	rotationalElements = frame['rotationalElements']
	bootDegreesAsRadians3(dst, rotationalElements[ 'ra'])
	bootDegreesAsRadians3(dst, rotationalElements['dec'])
	bootDegreesAsRadians3(dst, rotationalElements[ 'pm'])
	
	orbitalElements = frame['orbitalElements']
	bootUnsigned(dst, len(orbitalElements))

	for elements in orbitalElements:
		if 13 != len(elements):
			raise Exception('invalid number of elements in ephemeris file')
		index = 0
		for value in elements:
			onOrbitalElement[index](dst, value)
			index = index + 1

def bootFrameDynamicBody(dst, frame):
	bootUnsigned(dst, orbitalFrameType['DYNAMIC_BODY'])
	bootUnsigned(dst, frame['hid'])
	bootString(dst, frame['target'])
	bootString(dst, frame['description'])
	bootString(dst, frame['center'])

def bootFrameCamera(dst, frame):
	bootUnsigned(dst, orbitalFrameType['CAMERA_FRAME'])
	bootUnsigned(dst, frame['hid'])
	bootString(dst, frame['target'])
	bootString(dst, frame['description'])
	bootString(dst, frame['center'])

bootFrameType = {
	'DYNAMIC_POINT' : bootFrameDynamicPoint,
	 'ORBITAL_BODY' : bootFrameOrbitalBody,
	 'DYNAMIC_BODY' : bootFrameDynamicBody,
	 'CAMERA_FRAME' : bootFrameCamera,
}

def bootEphemerisLeapSeconds(dst, entry):
	bootUnsigned(dst, entry[0])
	bootUnsigned(dst, entry[1])
	bootUnsigned(dst, entry[2])
	bootDouble(dst, entry[3])
	
def bootEphemerisTime(dst, time):
	bootDouble(dst, time['TAI_TT'])
	bootDouble(dst, time['MA'][0])
	bootDouble(dst, time['MA'][1])
	bootDouble(dst, time['K'])
	bootDouble(dst, time['EB'])
	
	delta_at = time['DELTA_AT']
	bootUnsigned(dst, len(delta_at))
	for entry in delta_at:
		bootEphemerisLeapSeconds(dst, entry)

def bootEphemerisFrame(dst, frame):
	bootFrameType[frame['type']](dst, frame)

def bootEphemeris(dst, ephemeris):
	bootEphemerisTime(dst, ephemeris['time'])
	
	frames = ephemeris['frames']
	bootUnsigned(dst, len(frames))
	for frame in frames:
		bootEphemerisFrame(dst, frame)

def bootEphemerisFromFile(srcPath, dstPath):
	bootEphemeris(open(dstPath, 'wb'), json.load(open(srcPath)))

#
#	
#
bootContent = {
	 '.program' : bootProgramFromFile,
	'.material' : bootMaterialFromFile,
	  '.format' : bootVertexFormatFromFile,
	'.geometry' : bootGeometryFromFile,
	    '.mesh' : bootMeshFromFile,
		'.font' : bootFontFromFile,
      '.camera' : bootCameraFromFile,
	 '.context' : bootContextFromFile,
 '.starcatalog' : bootStarCatalogFromFile,
   '.ephemeris' : bootEphemerisFromFile,
}

#	bootstrap all
#
#
def bootAll(root):
	for path, paths, files in os.walk(root):
		for file in files:
			(name, ext) = os.path.splitext(file)
			if '.json' == ext:
				src = path + '\\' + file
				dst = path + '\\' + name

				kind = os.path.splitext(name)[1]

				print('bootstrapping: ' + src)
				bootContent[kind](src, dst)

#	main()
#
#
def main():
	optionParser = optparse.OptionParser()
	optionParser.add_option('-a', '--all', action = 'store_const', const = 'all', dest = 'kind')
	optionParser.add_option('-p', '--program', action = 'store_const', const = '.program', dest = 'kind')
	optionParser.add_option('-m', '--material', action = 'store_const', const = '.material', dest = 'kind')
	optionParser.add_option('-f', '--format', action = 'store_const', const = '.format', dest = 'kind')
	optionParser.add_option('-g', '--geometry', action = 'store_const', const = '.geometry', dest = 'kind')
	optionParser.add_option('-M', '--mesh', action = 'store_const', const = '.mesh', dest = 'kind')
	optionParser.add_option('-F', '--font', action = 'store_const', const = '.font', dest = 'kind')
	optionParser.add_option('-c', '--camera', action = 'store_const', const = '.camera', dest = 'kind')
	optionParser.add_option('-C', '--context', action = 'store_const', const = '.context', dest = 'kind')
	optionParser.add_option('-S', '--starcatalog', action = 'store_const', const ='.starcatalog', dest = 'kind')
	optionParser.add_option('-E', '--ephemeris', action = 'store_const', const ='.ephemeris', dest = 'kind')
	(opts, args) = optionParser.parse_args()

	if 'all' == opts.kind:
		if 1 != len(args):
			print('must specify <dir>')
			quit()
		bootAll(args[0])

	else:
		if 2 != len(args):
			print('must specify <src> and <dst> files')
			quit()

		bootContent[opts.kind](args[0], args[1])

#
#	script
#

if __name__ == '__main__':
	main()
