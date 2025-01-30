#pragma once

#include <string>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Types.h>

LUCID_GIGL_BEGIN

class Context;
class Material;

LUCID_GIGL_END

LUCID_GAL_BEGIN

class VertexFormat;
class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GUI_BEGIN

class Frame;

//	Renderer
//
//
class Renderer
{
public:
	Renderer() = default;

	virtual ~Renderer() = default;

	void initialize(std::string const &path);

	void shutdown();
	
	void operator()(LUCID_GIGL::Context const &context, Frame const *frame);

	void render(LUCID_GIGL::Context const &context, Frame const *frame);

	void add(Icon const &icon);

private:
	enum { BATCH_MAXIMUM = 2048 };

	std::vector<Icon> _icons;

	LUCID_GIGL::Material *_material = nullptr;

	LUCID_GAL::VertexFormat *_format = nullptr;
	LUCID_GAL::VertexBuffer *_vertices = nullptr;
	LUCID_GAL::IndexBuffer *_indices = nullptr;

	LUCID_GAL::VertexBuffer *_instances = nullptr;

	void process(Frame const *frame);

	LUCID_PREVENT_COPY(Renderer);
	LUCID_PREVENT_ASSIGNMENT(Renderer);
};

inline void Renderer::operator()(LUCID_GIGL::Context const &context, Frame const *frame)
{
	render(context, frame);
}

inline void Renderer::add(Icon const &icon)
{
	_icons.push_back(icon);
}

LUCID_GUI_END
