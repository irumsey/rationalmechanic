#!/usr/bin/env python3

import os
import optparse
import struct
import json
from functools import partial
from io import BytesIO

#
#
#

def tokenize(label):
	return label.split(':')
	
def type_of(label):
	tokens = tokenize(label)
	if 0  == len(tokens):
		raise Exception('improper member declaration')
	return tokens[0]

def name_of(label):
	tokens = tokenize(label)
	if 1 == len(tokens):
		return ''
	return tokens[1]

def write_raw(dst, fmt, value, write_prefix_data):
	if isinstance(value, list):
		if write_prefix_data:
			on_int32(dst, 'i32:count', len(value), True)
		for v in value:
			write_raw(dst, fmt, v, write_prefix_data)
	else:
		dst.write(struct.pack(f'{fmt}', value))
		
def on_bool(dst, name, value, write_prefix_data):
	write_raw(dst, '?', value, write_prefix_data)
	
def on_int8(dst, name, value, write_prefix_data):
	write_raw(dst, 'b', value, write_prefix_data)
	
def on_uint8(dst, name, value, write_prefix_data):
	if isinstance(value, str) and (1 == len(value)):
		value = ord(value)
	write_raw(dst, 'B', value, write_prefix_data)
	
def on_int16(dst, name, value, write_prefix_data):
	write_raw(dst, 'h', value, write_prefix_data)
	
def on_uint16(dst, name, value, write_prefix_data):
	write_raw(dst, 'H', value, write_prefix_data)
	
def on_int32(dst, name, value, write_prefix_data):
	write_raw(dst, 'i', value, write_prefix_data)
	
def on_uint32(dst, name, value, write_prefix_data):
	write_raw(dst, 'I', value, write_prefix_data)

def on_int64(dst, name, value, write_prefix_data):
	write_raw(dst, 'q', value, write_prefix_data)
	
def on_uint64(dst, name, value, write_prefix_data):
	write_raw(dst, 'Q', value, write_prefix_data)
	
def on_float32(dst, name, value, write_prefix_data):
	write_raw(dst, 'f', value, write_prefix_data)

def on_float64(dst, name, value, write_prefix_data):
	write_raw(dst, 'd', value, write_prefix_data)
      
def on_bin(dst, name, value, write_prefix_data):
	tokens = tokenize(name)
	if (len(tokens) < 2):
		raise Exception('binary chunk requires a subtype definition')
	buffer = BytesIO()
	on_type[tokens[1]](buffer, name, value, False)	
	if write_prefix_data:
		on_int32(dst, 'i32:count', buffer.getbuffer().nbytes, False)
	dst.write(buffer.getvalue())

def on_string(dst, name, value, write_prefix_data):
	if isinstance(value, list):
		if write_prefix_data:
			on_int32(dst, 'i32:count', len(value), False)
		for str in value:
			on_string(dst, name, str, write_prefix_data)
	else:
		size = len(value)
		if write_prefix_data:
			on_int32(dst, 'i32:count', size, False)
		dst.write(struct.pack(f'{size}s', bytes(value, 'ascii')))

def on_enumeration(dst, name, value, write_prefix_data, mapping):
	if isinstance(value, list):
		if write_prefix_data:
			on_int32(dst, 'i32:count', len(value), False)
		for v in value:
			on_enumeration(dst, name, v, write_prefix_data, mapping)
	else:
		on_int32(dst, name, mapping[value], False)
	
def on_reference(dst, name, value, write_prefix_data):
	if write_prefix_data:
		on_bool(dst, 'b:is_reference', True, write_prefix_data)
	on_string(dst, name, value, write_prefix_data)
	
def on_object(dst, name, value, write_prefix_data):
	if isinstance(value, list):
		if write_prefix_data:
			on_int32(dst, 'i32:count', len(value), False)
		for obj in value:
			on_object(dst, name, obj, write_prefix_data)
	else:
		if write_prefix_data:
			on_bool(dst, 'b:is_reference', False, write_prefix_data)
		for member, data in value.items():
			on_type[type_of(member)](dst, member, data, write_prefix_data)

def on_comment(dst, member, value, write_prefix_data):
	# do not serialize comments
	return

#
#
#
	
on_type = {
	  'b' : on_bool,
	 'i8' : on_int8,
	 'u8' : on_uint8,
	'i16' : on_int16,
	'u16' : on_uint16,
	'i32' : on_int32,
	'u32' : on_uint32,
	'i64' : on_int64,
	'u64' : on_uint64,
	'f32' : on_float32,
	'f64' : on_float64,
	'bin' : on_bin,
	'str' : on_string,
	'ref' : on_reference,
	'obj' : on_object,
	  '?' : on_comment,
}

#
#
#

def serialize(srcPath, dstPath):
	buffer = BytesIO()
	on_object(buffer, "obj:_root_", json.load(open(srcPath)), True)
	open(dstPath, 'wb').write(buffer.getvalue())
	print(f'serialized {srcPath}')
		
def serialize_all(root):
	for path, paths, files in os.walk(root):
		for file in files:
			(name, ext) = os.path.splitext(file)
			if '.json' == ext:
				srcPath = path + '\\' + file
				dstPath = path + '\\' + name
				serialize(srcPath, dstPath)

#
#
#
		
def main():
	optionParser = optparse.OptionParser()
	optionParser.add_option('-a', '--all', action = 'store_const', const = True, dest = 'all', default = False)
	optionParser.add_option('-e', '--enumfile', dest = 'enumfile')	
	(opts, args) = optionParser.parse_args()

	if opts.enumfile:
		enums = json.load(open(opts.enumfile))
		for type, values in enums.items():
			if type in on_type:
				raise Exception(f'{type} already defined')
			on_type[type] = partial(on_enumeration, mapping = values)
		
	if opts.all:
		if 1 != len(args):
			raise Exception('must specify directory')
		serialize_all(args[0])
	else:
		if 2 != len(args):
			raise Exception('must specify source and destination files')
		serialize(args[0], args[1])
			
if __name__ == '__main__':
	main()
