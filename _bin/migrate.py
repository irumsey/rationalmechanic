#
#	imports
#

import os
import optparse
import uuid
import json
import struct

def deg2rad(deg):
	return 0.01745329251994329576923690768489 * deg

def au2meter(au):
	return 149597870700.0 * au

#
#
#
primitiveLookup = [
	[   'boolean', "PRIMITIVE_BOOLEAN"   ,   "b:data", ],
	[   'integer', "PRIMITIVE_INTEGER"   , "i32:data", ],
	[     'float', "PRIMITIVE_FLOAT"     , "f32:data", ],
	[     'color', "PRIMITIVE_COLOR"     , "f32:data", ],
	[   'vector2', "PRIMITIVE_VECTOR2"   , "f32:data", ],
	[   'vector3', "PRIMITIVE_VECTOR3"   , "f32:data", ],
	[   'vector4', "PRIMITIVE_VECTOR4"   , "f32:data", ],
	['quaternion', "PRIMITIVE_QUATERNION", "f32:data", ],
	[ 'matrix2x2', "PRIMITIVE_MATRIX2X2" , "f32:data", ],
	[ 'matrix3x3', "PRIMITIVE_MATRIX3x3" , "f32:data", ],
	[ 'matrix4x4', "PRIMITIVE_MATRIX4x4" , "f32:data", ],
	[ 'texture2d', "PRIMITIVE_TEXTURE2D" , "str:data", ],
	[  'target2d', "PRIMITIVE_TARGET2D"  , "obj:data", ],
	[     'query', "PRIMITIVE_QUERY"     , "str:data", ],
]										 

#
#
#

def migrateSampler(sampler):
	sampler['str:name'] = sampler.pop('name')
	sampler['filterState:Filter'] = sampler.pop('Filter')
	sampler['addressMode:AddressU'] = sampler.pop('AddressU')
	sampler['addressMode:AddressV'] = sampler.pop('AddressV')
	sampler['addressMode:AddressW'] = sampler.pop('AddressW')
	sampler['f32:lodMipBias'] = sampler.pop('lodMipBias')
	sampler['i32:MaxAnisotropy'] = sampler.pop('MaxAnisotropy')
	sampler['comparison:Compare'] = sampler.pop('Compare')
	sampler['f32:BorderColor'] = sampler.pop('BorderColor')	
	sampler['f32:MinLOD'] = sampler.pop('MinLOD')
	sampler['f32:MaxLOD'] = sampler.pop('MaxLOD')

def migrateSamplers(samplers):
	for sampler in samplers:
		migrateSampler(sampler)

def migrateSamplersFromFile(srcPath, dstPath):
	samplers = json.load(open(srcPath))
	migrateSamplers(samplers)
	json.dump(samplers, open(dstPath, 'w'), indent = 4)

#
#
#

def migrateRenderStates(states):
	states['obj:RasterMode'] = states.pop('RasterMode')
	states['obj:BlendMode'] = states.pop('BlendMode')
	states['obj:DepthStencilMode'] = states.pop('DepthStencilMode')

	rasterMode = states['obj:RasterMode']
	rasterMode['fillMode:Fill'] = rasterMode.pop('Fill')
	rasterMode['cullMode:Cull'] = rasterMode.pop('Cull')
	rasterMode['b:Multisample'] = rasterMode.pop('Multisample')

	blendMode = states['obj:BlendMode']
	blendMode['u8:ColorWriteMask'] = blendMode.pop('ColorWriteMask')
	blendMode['b:Enabled'] = blendMode.pop('Enabled')
	blendMode['blendMode:SrcBlend'] = blendMode.pop('SrcBlend')
	blendMode['blendMode:SrcBlendAplha'] = blendMode.pop('SrcBlendAlpha')	
	blendMode['blendMode:DstBlend'] = blendMode.pop('DstBlend')
	blendMode['blendMode:DstBlendAplha'] = blendMode.pop('DstBlendAlpha')

	depthStencilMode = states['obj:DepthStencilMode']
	depthStencilMode['b:DepthEnabled'] = depthStencilMode.pop('DepthEnabled')
	depthStencilMode['deptWriteMask:DepthWrite'] = depthStencilMode.pop('DepthWrite')
	depthStencilMode['comparison:DepthFunction'] = depthStencilMode.pop('DepthFunction')
	depthStencilMode['b:StencilEnabled'] = depthStencilMode.pop("StencilEnabled")
	depthStencilMode['comparison:StencilFunction'] = depthStencilMode.pop('StencilFunction')	
	depthStencilMode['stencilOperation:StancilPass'] = depthStencilMode.pop('StencilPass')
	depthStencilMode['stencilOperation:StencilFail'] = depthStencilMode.pop('StencilFail')
	depthStencilMode['stencilOperation:StencilDepthFail'] = depthStencilMode.pop('StencilDepthFail')
	depthStencilMode['u8:StencilReadMask'] = depthStencilMode.pop('StencilReadMask')
	depthStencilMode['u8:StencilWriteMask'] = depthStencilMode.pop('StencilWriteMask')
	depthStencilMode['u8:StencilRef'] = depthStencilMode.pop('StencilRef')

def migrateRenderStatesFromFile(srcPath, dstPath):
	states = json.load(open(srcPath))
	migrateRenderStates(states)
	json.dump(states, open(dstPath, 'w'), indent = 4)

#
#
#

def migrateProgram(program):
	migrateSamplers(program['Samplers'])
	migrateRenderStates(program['RenderStates'])
	
	program['str:VertexShader'] = program.pop('VertexShader')

	if 'GeomatryShader' in program:
		program['str:GeometryShader'] = program.pop('GeometryShader')
	if 'PixelShader' in program:
		program['str:PixelShader'] = program.pop('PixelShader')

def migrateProgramFromFile(srcPath, dstPath):
	program = json.load(open(srcPath))
	migrateProgram(program)
	json.dump(program, open(dstPath, 'w'), indent = 4)

#
#
#

def migrateMaterialAttribute(attribute):
	attribute['str:name'] = attribute.pop('name')

	found = False
	for type in primitiveLookup:
		if type[0] in attribute:
			attribute['primitiveType:type'] = type[1]
			attribute[type[2]] = attribute.pop(type[0])
			found = True
			
	if not found:
		raise Exception('attribute type not found')

def migrateMaterialAttributes(attributes):
	for attribute in attributes:
		migrateMaterialAttribute(attribute)

def migrateMaterial(material):
	isReference = isinstance(material['program'], str)

	if isReference:
		material['ref:program'] = material.pop('program')
	else:
		material['obj:program'] = material.pop('program')
		migrateProgram(material['obj:program'])

	material['obj:attributes'] = material.pop('attributes')
	migrateMaterialAttributes(material['obj:attributes'])

def migrateMaterialFromFile(srcPath, dstPath):
	material = json.load(open(srcPath))
	migrateMaterial(material)
	json.dump(material, open(dstPath, 'w'), indent = 4)

#
#
#
def migrateVertexFormat(vertexFormat):
	vertexFormat['obj:elements'] = vertexFormat.pop('elements')
	for element in vertexFormat['obj:elements']:
		element['vertexElementFormat:format'] = element.pop('format')
		element['vertexElementType:type'] = element.pop('type')
		element['i32:location'] = element.pop('location')
		element['i32:stream'] = element.pop('stream')
		element['i32:offset'] = 4 * element.pop('offset')

def migrateVertexFormatFromFile(srcPath, dstPath):
	vertexFormat = json.load(open(srcPath))
	migrateVertexFormat(vertexFormat)
	json.dump(vertexFormat, open(dstPath, 'w'), indent = 4)

#
#
#
def migrateVertexBuffer(vertexBuffer):
	vertexBuffer['bufferUsage:usage'] = vertexBuffer.pop('usage')
	vertexBuffer['i32:stride'] = 4 * vertexBuffer.pop('stride')
	vertexBuffer['f32:data'] = vertexBuffer.pop('data')

def migrateVertexBufferFromFile(srcPath, dstPath):
	vertexBuffer = json.load(open(srcPath))
	migrateVertexBuffer(vertexBuffer)
	json.dump(vertexBuffer, open(dstPath, 'w'), indent = 4)

#
#
#
def migrateIndexBuffer(indexBuffer):
	indexBuffer['bufferUsage:usage'] = indexBuffer.pop('usage')
	indexBuffer['indexBufferFormat:format'] = indexBuffer.pop('format')

	if ('FORMAT_UINT16' == indexBuffer['indexBufferFormat:format']):
		indexBuffer['u16:data'] = indexBuffer.pop('data')
	else:
		indexBuffer['u32:data'] = indexBuffer.pop('data')

def migrateIndexBufferFromFile(srcPath, dstPath):
	indexBuffer = json.load(open(srcPath))
	migrateIndexBuffer(indexBuffer)
	json.dump(indexBuffer, open(dstPath, 'w'), index = 4)

#
#
#
def migrateGeometry(geometry):
	geometry['topology:topology'] = geometry.pop('topology')

	if isinstance(geometry['format'], str):		
		geometry['ref:format'] = geometry.pop('format')
	else:
		geometry['obj:format'] = geometry.pop('format')
		migrateVertexFormat(geometry['obj:format'])	

	geometry['obj:vertices'] = geometry.pop('vertices')
	migrateVertexBuffer(geometry['obj:vertices'])
	
	geometry['obj:indices'] = geometry.pop('indices')
	migrateIndexBuffer(geometry['obj:indices'])
			
def migrateGeometryFromFile(srcPath, dstPath):
	geometry = json.load(open(srcPath))
	migrateGeometry(geometry)
	json.dump(geometry, open(dstPath, 'w'), indent = 4)

#
#
#
def migrateMesh(mesh):
	if isinstance(mesh['material'], str):
		mesh['ref:material'] = mesh.pop('material')
	else:
		mesh['obj:material'] = mesh.pop('material')
		migrateMaterial(mesh['obj:material'])

	if isinstance(mesh['geometry'], str):
		mesh['ref:geometry'] = mesh.pop('geometry')
	else:
		mesh['obj:geometry'] = mesh.pop('geometry')
		migrateGeometry(mesh['obj:geometry'])

def migrateMeshFromFile(srcPath, dstPath):
	mesh = json.load(open(srcPath))
	migrateMesh(mesh)
	json.dump(mesh, open(dstPath, 'w'), indent = 4)

#
#
#	
def migrateFont(font):
	font['str:name'] = font.pop('name')
	font['f32:default spacing'] = font.pop('default spacing')
	font['ref:mesh'] = font.pop('mesh')
	font['obj:glyphs'] = font.pop('glyphs')
	
	for glyph in font['obj:glyphs']:
		glyph['u8:code'] = glyph.pop('code')
		glyph['f32:texcoord'] = glyph.pop('texcoord')
		glyph['f32:texsize'] = glyph.pop('texsize')
		glyph['f32:channel'] = glyph.pop('channel')

	font['f32:spacing'] = font.pop('spacing')
		
def migrateFontFromFile(srcPath, dstPath):
	font = json.load(open(srcPath))
	migrateFont(font)
	json.dump(font, open(dstPath, 'w'), indent = 4)

#
#
#

def migrateContextPrimitive(primitive):
	primitive['str:name'] = primitive.pop('name')

	found = False
	for type in primitiveLookup:
		if type[0] in primitive:
			primitive['primitiveType:type'] = type[1]
			primitive[type[2]] = primitive.pop(type[0])
			found = True
			
	if not found:
		raise Exception('primitive type not found')

def migrateContext(context):
	context['obj:primitives'] = context.pop('primitives')
	for primitive in context['obj:primitives']:
		migrateContextPrimitive(primitive)

def migrateContextFromFile(srcPath, dstPath):
	context = json.load(open(srcPath))
	migrateContext(context)
	json.dump(context, open(dstPath, 'w'), indent = 4)

#
#
#

def migrateStarCatalog(catalog):
	catalog['str:comments'] = catalog.pop('comments')
	catalog['obj:entries'] = catalog.pop('catalog')
	
	for entry in catalog['obj:entries']:
		entry['u32:XNO'] = entry.pop('XNO')
		entry['str:TYPE'] = entry.pop('TYPE')
		entry['f32:RA'] = entry.pop('RA')
		entry['f32:DEC'] = entry.pop('DEC')
		entry['f32:MAG'] = entry.pop('MAG')

def migrateStarCatalogFromFile(srcPath, dstPath):
	catalog = json.load(open(srcPath))
	migrateStarCatalog(catalog)
	json.dump(catalog, open(dstPath, 'w'), indent = 4)

def migrateFrameDynamicPoint(frame):
	frame['orbitalFrameType:type'] = frame.pop('type')
	frame['i32:hid'] = frame.pop('hid')
	frame['str:target'] = frame.pop('target')	
	frame['str:description'] = frame.pop('description')
	frame['str:center'] = frame.pop('center')

def migrateFrameOrbitalBody(frame):
	frame['orbitalFrameType:type'] = frame.pop('type')
	frame['i32:hid'] = frame.pop('hid')
	frame['str:target'] = frame.pop('target')	
	frame['str:description'] = frame.pop('description')
	frame['str:center'] = frame.pop('center')
	frame['obj:properties'] = frame.pop('properties')

	properties = frame['obj:properties']
	properties['obj:render'] = properties.pop('render')
	properties['obj:physical'] = properties.pop('physical')
	
	renderProperties = properties['obj:render']
	renderProperties['b:show orbit'] = renderProperties.pop('show orbit')
	renderProperties['b:show icon'] = renderProperties.pop('show icon')
	renderProperties['str:model'] = renderProperties.pop('model')
	renderProperties['obj:icon'] = renderProperties.pop('icon')
	renderProperties['obj:icon']['str:mesh'] = renderProperties['obj:icon'].pop('mesh')
	renderProperties['obj:icon']['f32:parameters'] = renderProperties['obj:icon'].pop('parameters')

	renderProperties['f32:channel0'] = renderProperties.pop('channel0')	
	renderProperties['f32:channel1'] = renderProperties.pop('channel1')	
	renderProperties['f32:channel2'] = renderProperties.pop('channel2')	
	
	physicalProperties = properties['obj:physical']

	physicalProperties['f64:GM'] = physicalProperties.pop('GM')
	physicalProperties['f64:mass'] = physicalProperties.pop('mass')
	physicalProperties['f64:radius'] = physicalProperties.pop('radius')

	frame['obj:rotationalElements'] = frame.pop('rotationalElements')
	rotationalElements = frame['obj:rotationalElements']
	
	rotationalElements['f64:ra'] = rotationalElements.pop('ra')
	rotationalElements['f64:dec'] = rotationalElements.pop('dec')
	rotationalElements['f64:pm'] = rotationalElements.pop('pm')
	
	frame['f32:orbitalElements'] = frame.pop('orbitalElements')

def migrateFrameDynamicBody(frame):
	frame['orbitalFrameType:type'] = frame.pop('type')
	frame['i32:hid'] = frame.pop('hid')
	frame['str:target'] = frame.pop('target')	
	frame['str:description'] = frame.pop('description')
	frame['str:center'] = frame.pop('center')

migrateFrameType = {
	'DYNAMIC_POINT' : migrateFrameDynamicPoint,
	 'ORBITAL_BODY' : migrateFrameOrbitalBody,
	 'DYNAMIC_BODY' : migrateFrameDynamicBody,
}
	
def migrateEphemerisTime(time):
	time['f64:TAI_TT'] = time.pop('TAI_TT')
	time['f64:MA'] = time.pop('MA')
	time['f64:K'] = time.pop('K')
	time['f64:EB'] = time.pop('EB')
	time['f64:DELTA_AT'] = time.pop('DELTA_AT')	

def migrateEphemerisFrame(frame):
	migrateFrameType[frame['type']](frame)

def migrateEphemeris(ephemeris):
	ephemeris['obj:time'] = ephemeris.pop('time')
	migrateEphemerisTime(ephemeris['obj:time'])
	
	ephemeris['obj:frames'] = ephemeris.pop('frames')
	for frame in ephemeris['obj:frames']:
		migrateEphemerisFrame(frame)

def migrateEphemerisFromFile(srcPath, dstPath):
	ephemeris = json.load(open(srcPath))
	migrateEphemeris(ephemeris)
	json.dump(ephemeris, open(dstPath, 'w'), indent = 4)

#
#	
#
migrateContent = {
	 '.program' : migrateProgramFromFile,
	'.material' : migrateMaterialFromFile,
	  '.format' : migrateVertexFormatFromFile,
	'.geometry' : migrateGeometryFromFile,
	    '.mesh' : migrateMeshFromFile,
		'.font' : migrateFontFromFile,
	 '.context' : migrateContextFromFile,
 '.starcatalog' : migrateStarCatalogFromFile,
   '.ephemeris' : migrateEphemerisFromFile,
}

#
#
#
def migrateAll(root):
	for path, paths, files in os.walk(root):
		for file in files:
			(name, ext) = os.path.splitext(file)
			if '.json' == ext:
				src = path + '\\' + file
				dst = "d:\\temp\\" + path + "\\" + name + '.json'

				kind = os.path.splitext(name)[1]

				print('migrating: ' + src)
				migrateContent[kind](src, dst)

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
		migrateAll(args[0])

	else:
		if 2 != len(args):
			print('must specify <src> and <dst> files')
			quit()

		migrateContent[opts.kind](args[0], args[1])

#
#	script
#

if __name__ == '__main__':
	main()
