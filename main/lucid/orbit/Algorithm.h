#pragma once

namespace lucid {
namespace orbit {

	class Frame;
	class DynamicPoint;
	class OrbitalBody;
	class DynamicBody;

	///
	///
	///
	class Algorithm
	{
	public:
		virtual ~Algorithm() = default;

		virtual void evaluate(DynamicPoint *point) = 0;

		virtual void evaluate(OrbitalBody *body) = 0;

		virtual void evaluate(DynamicBody *body) = 0;

	protected:
		Algorithm() = default;

	};

}	///	orbit
}	///	lucid
