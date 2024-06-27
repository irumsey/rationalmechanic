#pragma once

#include <unordered_map>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Algorithm.h>

///	test {
#include <lucid/gal/Types.h>
#include <vector>
///	} test

namespace lucid {
namespace orbit {

	class Frame;
	class DynamicPoint;
	class OrbitalBody;
	class DynamicBody;
	class CameraFrame;

	class Culler : public Algorithm
	{
	public:
		///	test {
		struct Visibility
		{
			enum STATE {
				STATE_PRUNED = 0,
				STATE_CULLED,
				STATE_IMPERCEPTIBLE,
				STATE_VISIBLE,
			};

			STATE state = STATE_CULLED;

			Visibility() = default;

			Visibility(Visibility const &) = default;

			Visibility(STATE state)
				: state(state)
			{
			}

			~Visibility() = default;

			Visibility &operator =(Visibility const &) = default;

		};

		std::unordered_map<size_t, Visibility> visibility;

		matrix4x4_t projMatrix;
		matrix4x4_t viewMatrix;

		scalar_t znear;
		scalar_t zfar;
		scalar_t aspect;

		scalar_t scaleFactor;

		scalar_t const znearInitial = 10.0;
		///	} test

		Culler();

		virtual ~Culler();

		void cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant);

		virtual void evaluate(DynamicPoint *point) override;

		virtual void evaluate(OrbitalBody *body) override;

		virtual void evaluate(DynamicBody *body) override;

		virtual void evaluate(CameraFrame *camera) override;

	private:
		scalar_t _interpolant;
		matrix4x4_t _viewProjMatrix;
		frustum_t _frustum;

		void cull(Frame *frame);

	};

}	/// orbit
}	///	lucid