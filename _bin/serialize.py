import os
import optparse
import struct
import json
from functools import partial

#
#
#

def tokenize(label):
	return label.split(':')
	
def type_of(label):
	return tokenize(label)[0]

def name_of(label):
	return tokenize(label)[1]

def write_raw(dst, fmt, value):
	if isinstance(value, list):
		count = len(value)
		write_uint64(dst, count)
		dst.write(struct.pack(f'{count}{fmt}', *value))
	else:
		dst.write(struct.pack(f'{fmt}', value))
		
def write_bool(dst, value):
	write_raw(dst, '?', value)
	
def write_int8(dst, value):
	write_raw(dst, 'b', value)
	
def write_uint8(dst, value):
	write_raw(dst, 'B', value)
	
def write_int16(dst, value):
	write_raw(dst, 'h', value)
	
def write_uint16(dst, value):
	write_raw(dst, 'H', value)
	
def write_int32(dst, value):
	write_raw(dst, 'i', value)
	
def write_uint32(dst, value):
	write_raw(dst, 'I', value)

def write_int64(dst, value):
	write_raw(dst, 'q', value)
	
def write_uint64(dst, value):
	write_raw(dst, 'Q', value)
	
def write_float32(dst, value):
	write_raw(dst, 'f', value)

def write_float64(dst, value):
	write_raw(dst, 'd', value)
      
def write_string(dst, str):
	if isinstance(str, list):
		count = len(str)
		write_uint64(dst, count)
		for s in str:
			write_string(dst, s)
	else:
		size = len(str)
		write_uint64(dst, size)
		dst.write(struct.pack(f'{size}s', bytes(str, 'ascii')))

def write_enumeration(dst, value, mapping):
	write_int32(dst, mapping[value])

def write_reference(dst, path):
	write_bool(dst, False)
	write_string(dst, path)
	
def write_object(dst, obj):
	if isinstance(obj, list):
		count = len(obj)
		write_uint64(dst, count)
		for o in obj:
			write_object(dst, o)
	else:
		write_bool(dst, True)
		for member, data in obj.items():
			write_value[type_of(member)](dst, data)
			
#
#
#
				
def serialize_all(root):
	for path, paths, files in os.walk(root):
		for file in files:
			(name, ext) = os.path.splitext(file)
			if '.json' == ext:
				srcPath = path + '\\' + file
				dstPath = path + '\\' + name
				serialize(srcPath, dstPath)
				print(f'serialized {srcPath} as {dstPath}')
	
def serialize(srcPath, dstPath):
	write_object(open(dstPath, 'wb'), json.load(open(srcPath)))
	print(f'serialized {srcPath} as {dstPath}')

#
#
#
	
write_value = {
	  'b' : write_bool,
	 'i8' : write_int8,
	 'u8' : write_uint8,
	'i16' : write_int16,
	'u16' : write_uint16,
	'i32' : write_int32,
	'u32' : write_uint32,
	'i64' : write_int64,
	'u64' : write_uint64,
	'f32' : write_float32,
	'f64' : write_float64,
	'str' : write_string,
	'ref' : write_reference,
	'obj' : write_object,
}
	
def main():
	optionParser = optparse.OptionParser()
	optionParser.add_option('-a', '--all', action = 'store_const', const = True, dest = 'all', default = False)
	optionParser.add_option('-e', '--enumfile', dest = 'enumfile')	
	(opts, args) = optionParser.parse_args()

	if opts.enumfile:
		enums = json.load(open(opts.enumfile))
		for type, values in enums.items():
			if type in write_value:
				print(f'{type} already defined')
				quit
			write_value[type] = partial(write_enumeration, mapping = values)
		
	if opts.all:
		if 1 != len(args):
			print('must specify <dir>')
			quit()
		serialize_all(args[0])
	else:
		if 2 != len(args):
			print('must specify <src> and <dst> files')
			quit()
		serialize(args[0], args[1])
			
if __name__ == '__main__':
	main()
