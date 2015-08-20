#pragma once

#include <string>
#include <list>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Factory.h>

///
///
///

class Test;

///
///
///
class Tests
{
public:
	Tests();

	virtual ~Tests();

	Test *nextTest();

private:
	lucid::core::Factory<std::string,Test> _factory;
	std::list<std::string> _tests;

	LUCID_PREVENT_COPY(Tests);
	LUCID_PREVENT_ASSIGNMENT(Tests);
};

inline Test *Tests::nextTest()
{
	if (_tests.empty())
	{
		return nullptr;
	}

	std::string test = _tests.front();
	_tests.pop_front();

	return _factory.create(test);
}
