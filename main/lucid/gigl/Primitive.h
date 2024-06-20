#pragma once

#include <memory>
#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Types.h>

///
///
///
namespace lucid {
namespace core {

	class Reader;

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {

	class Unordered2D;
	class Texture2D;
	class RenderTarget2D;

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gigl {

	///	Primitive
	///
	///	basic "variant" data type used by material and context.
	class Primitive final
	{
	public:
		typedef std::shared_ptr<::lucid::gal::   Unordered2D> shared_unordered2d_t;
		typedef std::shared_ptr<::lucid::gal::     Texture2D>   shared_texture2d_t;
		typedef std::shared_ptr<::lucid::gal::RenderTarget2D>    shared_target2d_t;

		struct UNKNOWN final {};
		struct UNDEFINED final {};

		template<class T> struct Type                       { enum { VALUE =  0 }; };
		template<>        struct Type<             UNKNOWN> { enum { VALUE =  0 }; };
		template<>        struct Type<           UNDEFINED> { enum { VALUE =  1 }; };
		template<>        struct Type<                bool> { enum { VALUE =  2 }; };
		template<>        struct Type<             int32_t> { enum { VALUE =  3 }; };
		template<>        struct Type<              Scalar> { enum { VALUE =  4 }; };
		template<>        struct Type<               Color> { enum { VALUE =  5 }; };
		template<>        struct Type<             Vector2> { enum { VALUE =  6 }; };
		template<>        struct Type<             Vector3> { enum { VALUE =  7 }; };
		template<>        struct Type<             Vector4> { enum { VALUE =  8 }; };
		template<>        struct Type<          Quaternion> { enum { VALUE =  9 }; };
		template<>        struct Type<           Matrix2x2> { enum { VALUE = 10 }; };
		template<>        struct Type<           Matrix3x3> { enum { VALUE = 11 }; };
		template<>        struct Type<           Matrix4x4> { enum { VALUE = 12 }; };
		template<>        struct Type<  shared_texture2d_t> { enum { VALUE = 13 }; };
		template<>        struct Type<   shared_target2d_t> { enum { VALUE = 14 }; };
		template<>        struct Type<     std::    string> { enum { VALUE = 15 }; };

		template<>        struct Type<shared_unordered2d_t> { enum { VALUE = 16 }; };

		enum { TYPE_COUNT = 17 };

		struct Info
		{
			int32_t const tid;
			char const * const name;
		};

		Primitive() = default;

		virtual ~Primitive() = default;

		template<class T> Primitive(T const &rhs)
		{
			_tid = Type<T>::VALUE;
			_any.reset(new Specific<T>(rhs));
		}

		Primitive(::lucid::core::Reader &reader)
		{
			readFrom(reader);
		}

		Primitive(Primitive const &rhs)
		{
			coerceFrom(rhs);
		}

		template<class T> Primitive &operator=(T const &rhs)
		{
			asSpecific<T>(_any)->value = rhs;
			return *this;
		}

		Primitive &operator=(Primitive const &rhs)
		{
			if (this != &rhs)
				coerceFrom(rhs);
			return *this;
		}

		template<class T> T const &as() const
		{
			return asSpecific<T>(_any)->value;
		}

		template<class T> T &as()
		{
			return asSpecific<T>(_any)->value;
		}

		Info const &info() const;

	private:
		///
		///
		///
		struct Any
		{
			Any() = default;

			virtual ~Any() = default;

			virtual Any *clone() const = 0;

			LUCID_PREVENT_COPY(Any);
			LUCID_PREVENT_ASSIGNMENT(Any);
		};

		///
		///
		///
		template<class T> struct Specific : public Any
		{
			T value;

			Specific() = default;

			virtual ~Specific() = default;

			Specific(T const &value)
				: value(value)
			{
			}

			virtual Any *clone() const override
			{
				return new Specific(value);
			}

			LUCID_PREVENT_COPY(Specific);
			LUCID_PREVENT_ASSIGNMENT(Specific);
		};

		static Info const infos[TYPE_COUNT];

		int32_t _tid = Type<UNDEFINED>::VALUE;
		std::unique_ptr<Any> _any;

		void coerceFrom(Primitive const &rhs);

		void readFrom(::lucid::core::Reader &reader);

		template<class T> static Specific<T> *asSpecific(std::unique_ptr<Any> const &rhs)
		{
			typedef Specific<T> specific_t;

			specific_t *specific = dynamic_cast<specific_t*>(rhs.get());
			LUCID_VALIDATE(nullptr != specific, "parameter type mismatch");

			return specific;
		}

	};

	inline Primitive::Info const &Primitive::info() const
	{
		return infos[_tid];
	}

}	///	gigl
}	///	lucid
