#include "Session.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/AABB.h>
#include <algorithm>

#ifdef min
#	undef min
#endif

namespace math = ::lucid::math;
namespace  gal = ::lucid:: gal;
namespace gigl = ::lucid::gigl;
namespace   rm = ::lucid::  rm;

namespace /* anonymous */ {

	typedef math::AABB<float32_t, 3> Box;

	template<class T> inline void safe_delete(T *&ptr)
	{
		delete ptr;
		ptr = nullptr;
	}

	inline gal::Vector3 hashPosition(Box const &&box, size_t index)
	{
		gal::Vector3 pos = box.center();
		if (0 == index)
			return pos;

		float32_t half = math::len(0.5f * (box.max - box.min));

		gal::Vector3 const lo[] = {
			gal::Vector3(0, 0, 0),
			gal::Vector3(1, 0, 0),
			gal::Vector3(1, 1, 0),
			gal::Vector3(0, 1, 0),
			gal::Vector3(0, 0, 1),
			gal::Vector3(1, 0, 1),
			gal::Vector3(1, 1, 1),
			gal::Vector3(0, 1, 1),
		};

		gal::Vector3 const hi[] = {
			gal::Vector3(1, 1, 1),
			gal::Vector3(0, 1, 1),
			gal::Vector3(0, 0, 1),
			gal::Vector3(1, 0, 1),
			gal::Vector3(1, 1, 0),
			gal::Vector3(0, 1, 0),
			gal::Vector3(0, 0, 0),
			gal::Vector3(1, 0, 0),
		};

		return hashPosition(Box(pos - half * lo[0x07 & index], pos + half * hi[0x07 & index]), index >> 3);
	}

}

void Session::initialize()
{
	shutdown();

	_context = gigl::Context("content/render.context");
	
	_beam = gigl::Mesh::create("content/beam.mesh");
	_edgeInstances = gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, INSTANCE_MAXIMUM, sizeof(EdgeInstance));

	_sprite = gigl::Mesh::create("content/sprite.mesh");
	_nodeInstances = gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, INSTANCE_MAXIMUM, sizeof(NodeInstance));

	_mind = new rm::Mind(200, 1000);

	_initialized = true;
}

void Session::shutdown()
{
	safe_delete(_mind);

	safe_delete(_nodeInstances);
	safe_delete(_sprite);

	safe_delete(_edgeInstances);
	safe_delete(_beam);

	_initialized = false;
}

void Session::update(float64_t t, float64_t dt)
{
	if (!_initialized)
		return;

	gal::Vector3 viewPosition = gal::Vector3(20, 15, 25);

	gal::Matrix4x4 viewMatrix = math::look(viewPosition, gal::Vector3(), gal::Vector3(0, 0, 1));
	gal::Matrix4x4 projMatrix = math::perspective(0.78f, 1.78f, 1.f, 100.f);
	gal::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;

	_context["lightPosition"] = gal::Vector3(100, -100, 100);

	_context["viewPosition"] = viewPosition;

	_context["viewUp"] = gal::Vector3(viewMatrix.xx, viewMatrix.xy, viewMatrix.xz);
	_context["viewRight"] = gal::Vector3(viewMatrix.yx, viewMatrix.yy, viewMatrix.yz);
	_context["viewForward"] = gal::Vector3(-viewMatrix.zx, -viewMatrix.zy, -viewMatrix.zz);

	_context["viewMatrix"] = viewMatrix;
	_context["invViewMatrix"] = math::inverse(viewMatrix);

	_context["projMatrix"] = projMatrix;
	_context["invProjMatrix"] = math::inverse(projMatrix);

	_context["viewProjMatrix"] = viewProjMatrix;
	_context["invViewProjMatrix"] = math::inverse(viewProjMatrix);

	_mind->update(1000);
	_mind->execute(_mind->chromosome(), 1000);

	rm::Graph &graph = _mind->graph();
	
	_nodeCount = std::min(graph.nodeCount(), size_t(INSTANCE_MAXIMUM));
	_edgeCount = 0;

	std::list<size_t> stack;

	EdgeInstance *edgeInstances = (EdgeInstance *)(_edgeInstances->lock());
	NodeInstance *nodeInstances = (NodeInstance *)(_nodeInstances->lock());
	for (size_t i = 0; i < _nodeCount; ++i)
	{
		rm::Node const &node = graph.getNode(i);
		NodeInstance &nodeInstance = nodeInstances[i];

		gal::Vector3 position = hashPosition(Box(gal::Vector3(-5, -5, -5), gal::Vector3(5, 5, 5)), i);

		nodeInstance.position[0] = nodeInstance.position[1] = position;
		nodeInstance.scale[0] = nodeInstance.scale[1] = 0.5f;
		nodeInstance.diffuse = gal::Color(0, 0, 1, 1);

		stack.clear();
		graph.pushDownstream(stack, i);
		for (auto iter = stack.begin(); (iter != stack.end()) && (_edgeCount < INSTANCE_MAXIMUM); ++iter, ++_edgeCount)
		{
			EdgeInstance &edgeInstance = edgeInstances[_edgeCount];
			edgeInstance.position[0] = position;
			edgeInstance.position[1] = hashPosition(Box(gal::Vector3(-5, -5, -5), gal::Vector3(5, 5, 5)), *iter);
			edgeInstance.diffuse = gal::Color(0, 0, 1, 1);
		}
	}
	_nodeInstances->unlock();
	_edgeInstances->unlock();
}

void Session::render(float64_t t, float32_t interpolant)
{
	if (!_initialized)
		return;

	gal::Pipeline &pipeline = gal::Pipeline::instance();

	_context["time"] = float32_t(t);
	_context["interpolant"] = float32_t(interpolant);

	if (0 != _edgeCount)
	{
		pipeline.setVertexStream(1, _edgeInstances);
		_beam->renderInstanced(_context, int32_t(_edgeCount));
	}

	if (0 != _nodeCount)
	{
		pipeline.setVertexStream(1, _nodeInstances);
		_sprite->renderInstanced(_context, int32_t(_nodeCount));
	}
}
