#pragma once

#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Types.h>

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

	void initialize();

	void shutdown();
	
	void operator()(Frame const *frame);

	void render(Frame const *frame);

	void add(Icon const &icon);

private:
	enum { BATCH_MAXIMUM = 2048 };

	std::vector<Icon> _icons;

	LUCID_GAL::VertexFormat *_format = nullptr;
	LUCID_GAL::VertexBuffer *_vertices = nullptr;
	LUCID_GAL::IndexBuffer *_indices = nullptr;

	LUCID_GAL::VertexBuffer *_instances = nullptr;

	void capusa(Frame const *frame);

	LUCID_PREVENT_COPY(Renderer);
	LUCID_PREVENT_ASSIGNMENT(Renderer);
};

inline void Renderer::operator()(Frame const *frame)
{
	render(frame);
}

inline void Renderer::add(Icon const &icon)
{
	_icons.push_back(icon);
}

LUCID_GUI_END
