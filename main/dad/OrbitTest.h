#pragma once

#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/gal/Parameter.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>
#include <dad/Test.h>

///
///
///
namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

struct MouseEvent;

struct Body
{
	size_t id = 0;
	size_t cid = 0;

	std::string name;

	::lucid::orbit::vector3_t position;
	::lucid::orbit::vector3_t velocity;

	::lucid::orbit::Properties properties;
	::lucid::orbit::Elements elements;
};

///
///
///
class OrbitTest : public Test
{
public:
	OrbitTest() = default;

	virtual ~OrbitTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual void onInput(MouseEvent const &event) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	typedef ::lucid::math::Vector<float32_t,2> Vector2;
	typedef ::lucid::math::Vector<float32_t,3> Vector3;
	typedef ::lucid::math::Vector<float32_t,4> Vector4;
	typedef ::lucid::math::Matrix<float32_t,4,4> Matrix3x3;
	typedef ::lucid::math::Matrix<float32_t,4,4> Matrix4x4;

	struct RenderParameters
	{
		lucid::gal::Parameter const           *hu = nullptr;
		lucid::gal::Parameter const *eccentricity = nullptr;
		lucid::gal::Parameter const       *domain = nullptr;
		lucid::gal::Parameter const    *lineColor = nullptr;
		lucid::gal::Parameter const  *worldMatrix = nullptr;
	};

	///	maximum per draw call (not total maximum)
	enum { SPHERE_MAXIMUM = 100 };

	bool _passed = true;

	std::unordered_map<size_t, Body> _bodies;

	lucid::gigl::Context _context;

	Vector3 _viewPosition;
	Vector3 _viewDirection;

	std::shared_ptr<lucid::gigl::Mesh> _sphereMesh;
	std::shared_ptr<lucid::gal::VertexBuffer> _sphereInstances;

	std::shared_ptr<lucid::gigl::Mesh> _maskMesh;
	std::shared_ptr<lucid::gal::VertexBuffer> _maskInstances;

	std::shared_ptr<lucid::gigl::Mesh> _orbitMesh;
	RenderParameters _renderParameters;

	void addBody(std::string const &name, float32_t jdn);

	void renderOrbits(float32_t time, float32_t interpolant) const;

	void renderOrbit(Body const &target) const;

	void renderMasks(float32_t time, float32_t interpolant) const;

	void renderBodies(float32_t time, float32_t interpolant) const;

	Vector3 computeAbsoluePosition(size_t target) const;

	LUCID_PREVENT_COPY(OrbitTest);
	LUCID_PREVENT_ASSIGNMENT(OrbitTest);
};

inline bool OrbitTest::passed() const
{
	return _passed;
}
