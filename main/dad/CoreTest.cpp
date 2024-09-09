#include "CoreTest.h"
#include "Utility.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/core/FileWriter.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Factory.h>
#include <lucid/core/Cache.h>
#include <lucid/core/Clock.h>
#include <lucid/core/Ctor.h>
#include <lucid/core/Array.h>
#include <lucid/core/Bits.h>
#include <lucid/core/Logger.h>
#include <lucid/core/String.h>
#include <lucid/core/Types.h>

///
///
///

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

///
///
///

void CoreTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting core test...");
}

bool CoreTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	//
	//	test the static array
	//
	{
		LUCID_CORE::Array<256, int> a(101);
		LUCID_CORE::Array<256, int> b = a;

		_passed &= (101 == b[128]);
	}

	//
	//
	// 
	{
		uint16_t a = 101;
		uint16_t b = 548;

		size_t d = LUCID_CORE::bits::hamming_distance(a, b);

		_passed &= (3 == d);
	}

	//
	// 
	// 
	{
		LUCID_CORE::Cache<int, float> cache;
		LUCID_CORE::CtorCopy<float> ctorPrototype(new float(3.14159f));

		std::shared_ptr<float> value = cache.get(5, ctorPrototype);
		std::shared_ptr<float> again = cache.get(5, ctorPrototype);

		_passed = (value == again);
		_passed = (3.14159f == *again);
	}

	//
	// 
	// 
	{
		std::unique_ptr<LUCID_CORE::Clock> clock(LUCID_CORE::Clock::create());
		float64_t time = clock->time();
		// not much to do...
	}

	//
	// 
	// 
	{
		struct Base
		{
			int value = 0;

			Base(int value) : value(value) {}

			virtual ~Base() = default;
		};

		struct Derived : public Base
		{
			Derived() : Base(101) {}

			virtual ~Derived() = default;
		};

		struct Prototype : public Base
		{
			Prototype(int value) : Base(value) {}

			virtual ~Prototype() = default;
		};

		LUCID_CORE::Factory<std::string, Base> ftor;

		ftor.add("101", new LUCID_CORE::CtorStd<Derived, Base>());
		ftor.add("copy", new LUCID_CORE::CtorCopy<Prototype, Base>(new Prototype(21)));

		std::unique_ptr<Base> a(ftor.create("101"));
		std::unique_ptr<Base> b(ftor.create("copy"));

		_passed &= (101 == a->value);
		_passed &= (21 == b->value);
	}

	//
	//
	// 
	{
		std::string testString = "This is a test of writing/reading a string to/from file.";

		LUCID_CORE::FileWriter writer("writeReadTest.bin");
		writer.write(testString);
		writer.write(int32_t(101));
		writer.write(float32_t(3.14159f));
		writer.close();

		LUCID_CORE::FileReader cReader("writeReadTest.bin");
		LUCID_CORE::Reader &reader = cReader;

		std::string readString = reader.read<std::string>();
		int32_t intValue = reader.read<int32_t>();
		float32_t floatValue = reader.read<float32_t>();
		cReader.close();
	}

	// TBD: tests here...

	return true;
}

void CoreTest::render(float32_t time, float32_t interpolant)
{
	LUCID_GAL_PIPELINE.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
