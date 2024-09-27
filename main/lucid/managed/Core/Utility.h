#pragma once

#define LUCID_MANAGED_PROPERTY(name, type)												\
	property type name {																\
		type get() { return _internal->name; }											\
		void set(type value) { _internal->name = value; }								\
	}

#define LUCID_MANAGED_PROPERTY_EX(nameManaged, nameNative, type)						\
	property type nameManaged {															\
		type get() { return _internal->nameNative; }									\
		void set(type value) { _internal->nameNative = value; }							\
	}

#define LUCID_MANANGED_ENUMERATION(nameManaged, nameNative, typeManaged, typeNative)	\
	property typeManaged nameManaged {													\
		typeManaged get() {	return typeManaged(_internal->nameNative); }				\
		void set(typeManaged value) { _internal->nameNative = typeNative(value); }		\
	}