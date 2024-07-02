#pragma once

#include <cassert>
#include <string>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>

LUCID_CORE_BEGIN

///	Ctor
///
///	base constructor.
template<class Base> class Ctor
{
public:
	virtual ~Ctor() = 0 {}

	virtual Base *create() const = 0;

protected:
	Ctor() {}

	LUCID_PREVENT_COPY(Ctor);
	LUCID_PREVENT_ASSIGNMENT(Ctor);
};

///	CtorStd
///
///	standard constructor which simply creates a new instance of type Derived.
template<class Derived, class Base = Derived> class CtorStd : public Ctor<Base>
{
public:
	CtorStd()
	{
	}

	virtual ~CtorStd()
	{
	}

	virtual Base *create() const
	{
		return new Derived;
	}

	Base *operator()() const
	{
		return create();
	}

	LUCID_PREVENT_COPY(CtorStd);
	LUCID_PREVENT_ASSIGNMENT(CtorStd);
};

///	CtorResource
///
///	used to create an instance by calling Derived::create(<path>).
template<class Derived, class Base = Derived> class CtorResource : public Ctor<Base>
{
public:
	CtorResource(std::string const &path)
		: _path(path)
	{
	}

	virtual ~CtorResource()
	{
	}

	virtual Base *create() const
	{
		return Derived::create(_path);
	}

	Base *operator()() const
	{
		return create();
	}

private:
	std::string _path;

	LUCID_PREVENT_COPY(CtorResource);
	LUCID_PREVENT_ASSIGNMENT(CtorResource);
};

///	CtorCopy
///
///	constructor which creates a copy of supplied "prototype".
///
///	note: takes ownership of supplied prototype.
template<class Derived, class Base = Derived> class CtorCopy : public Ctor<Base>
{
public:
	CtorCopy(Derived const *prototype)
		: _prototype(prototype)
	{
		assert(_prototype);
	}

	virtual ~CtorCopy()
	{
		delete _prototype;
	}

	virtual Base *create() const
	{
		return new Derived(*_prototype);
	}

	Base *operator()() const
	{
		return create();
	}

private:
	Derived const *_prototype;

	LUCID_PREVENT_COPY(CtorCopy);
	LUCID_PREVENT_ASSIGNMENT(CtorCopy);
};

LUCID_CORE_END