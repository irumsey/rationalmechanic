#include "Renderer.h"
#include "Frame.h"
#include <lucid/gigl/Font.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Context.h>
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/Memory.h>
#include <algorithm>

LUCID_ANONYMOUS_BEGIN

LUCID_GAL::VertexElement const format[] = {
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT2, LUCID_GAL::VertexElement::  TYPE_VERTEX, 0, 0,  0, },
	{ LUCID_GAL::VertexElement:: FORMAT_UINT1, LUCID_GAL::VertexElement::TYPE_INSTANCE, 1, 1,  0, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT4, LUCID_GAL::VertexElement::TYPE_INSTANCE, 2, 1,  4, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT4, LUCID_GAL::VertexElement::TYPE_INSTANCE, 3, 1, 20, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT4, LUCID_GAL::VertexElement::TYPE_INSTANCE, 4, 1, 36, },
};

LUCID_GAL::Vector2 const vertices[] = {
	{ 0.f, 0.f, },
	{ 0.f, 1.f, },
	{ 1.f, 0.f, },
	{ 1.f, 1.f, },
};

uint16_t const indices[] = { 0, 1, 2, 3, };

LUCID_ANONYMOUS_END

LUCID_GUI_BEGIN

void Renderer::initialize(std::string const &font, std::string const &material)
{
	shutdown();

	_font = LUCID_GIGL::Font::create(font);
	_characters = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, TEXT_DRAW_MAXIMUM, sizeof(Character));

	_material = LUCID_GIGL::Material::create(material);

	_format = LUCID::gal::VertexFormat::create(::format, 5);

	_vertices = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_STATIC, 4, sizeof(LUCID_GAL::Vector2));
	_indices = LUCID_GAL::IndexBuffer::create(LUCID_GAL::IndexBuffer::USAGE_STATIC, LUCID_GAL::IndexBuffer::FORMAT_UINT16, 4);
	_instances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, ICON_DRAW_MAXIMUM, sizeof(Icon));
	
	std::memcpy(_vertices->lock(), ::vertices, 4 * sizeof(LUCID_GAL::Vector2));
	_vertices->unlock();

	std::memcpy(_indices->lock(), ::indices, 4 * sizeof(uint16_t));
	_indices->unlock();
}

void Renderer::shutdown()
{
	LUCID_CORE::reset_raw_ptr(_instances);
	LUCID_CORE::reset_raw_ptr(_indices);
	LUCID_CORE::reset_raw_ptr(_vertices);
	LUCID_CORE::reset_raw_ptr(_format);
	LUCID_CORE::reset_raw_ptr(_material);
	LUCID_CORE::reset_raw_ptr(_characters);
	LUCID_CORE::reset_raw_ptr(_font);
}

void Renderer::render(LUCID_GIGL::Context const &context, Frame const *frame)
{
	LUCID_PROFILE_SCOPE("gui::Renderer::render()");

	_text.clear();
	_icons.clear();

	process(frame);

	renderIcons(context);
	renderText(context);
}

void Renderer::add(ALIGNMENT align, Point const &point, float32_t size, std::string const &text, Color const &color)
{
	size_t i = _text.size();
	_text.resize(i + text.size());

	LUCID_GAL::Vector2 position(float32_t(point.x), float32_t(point.y));

	_font->typeset(&(_text[i]), align, position, size, text, color);
}

void Renderer::process(Frame const *frame)
{
	LUCID_PROFILE_SCOPE("gui::Renderer::process()");

	if (nullptr == frame)
		return;

	frame->accept(this);

	process(frame->nextSibling());
	process(frame->firstChild());
}

void Renderer::renderIcons(LUCID_GIGL::Context const &context)
{
	LUCID_PROFILE_SCOPE("gui::Renderer::renderIcons()");

	_material->begin(context);

		size_t pos = 0;
		size_t count = std::min(size_t(ICON_DRAW_MAXIMUM), _icons.size() - pos);
		while (0 < count)
		{
			Icon *buffer = _instances->lock_as<Icon>();
			std::memcpy(buffer, &(_icons[pos]), count * sizeof(Icon));
			_instances->unlock();

			LUCID_GAL_PIPELINE.beginGeometry(_format);
				LUCID_GAL_PIPELINE.setVertexStream(0, _vertices);
				LUCID_GAL_PIPELINE.setVertexStream(1, _instances);
				LUCID_GAL_PIPELINE.setIndexStream(_indices);
				LUCID_GAL_PIPELINE.drawInstanced(LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_STRIP, 4, 4, int(count));
			LUCID_GAL_PIPELINE.endGeometry(_format);

			pos = pos + count;
			count = std::min(size_t(ICON_DRAW_MAXIMUM), _icons.size() - pos);
		}

	_material->end();
}

void Renderer::renderText(LUCID_GIGL::Context const &context)
{
	LUCID_PROFILE_SCOPE("gui::Renderer::renderText()");

	size_t pos = 0;
	size_t count = std::min(size_t(TEXT_DRAW_MAXIMUM), _text.size() - pos);
	while (0 < count)
	{
		Character *buffer = _characters->lock_as<Character>();
		std::memcpy(buffer, &(_text[pos]), count * sizeof(Character));
		_characters->unlock();

		LUCID_GAL_PIPELINE.setVertexStream(1, _characters);
		_font->renderInstanced(context, int(count));

		pos = pos + count;
		count = std::min(size_t(TEXT_DRAW_MAXIMUM), _text.size() - pos);
	}
}

LUCID_GUI_END
