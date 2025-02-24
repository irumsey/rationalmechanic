#include "Font.h"
#include "Resources.h"
#include <lucid/core/Unserializer.h>
#include <lucid/core/Error.h>

LUCID_GIGL_BEGIN

///
///
///

Font *Font::create(std::string const &path)
{
	return Font::create(LUCID_CORE::Unserializer(path));
}

Font *Font::create(LUCID_CORE::Unserializer &reader)
{
	return new Font(reader);
}

///
///
///

Font::Font(LUCID_CORE::Unserializer &reader)
{
	initialize(reader);
}

Font::~Font()
{
	shutdown();
}

size_t Font::typeset(Character *buffer, ALIGNMENT align, LUCID_GAL::Vector2 const &position, float32_t size, std::string const &text, LUCID_GAL::Color const &color)
{
	size_t const count = text.size();

	uint8_t prevCode = 0;
	float32_t textWidth = 0.f;
	float32_t x = position.x;

	if (ALIGN_LEFT != align)
	{
		for (size_t i = 0; i < count; ++i)
			textWidth += size * spacing(prevCode, text[i]);
		x = (ALIGN_RIGHT == align) ? x - textWidth : x - 0.5f * textWidth;
	}

	prevCode = 0;

	for (size_t i = 0; i < count; ++i)
	{
		Glyph const &glyph = lookup(text[i]);
		Character &character = buffer[i];

		float32_t dx = (0 == i) ? 0.f : size * spacing(prevCode, glyph.code);
		x = x + dx;

		character.position = LUCID_GAL::Vector4(x, position.y, size, size);
		character.texcoord = LUCID_GAL::Vector4(glyph.texcoord.x, glyph.texcoord.y, glyph.texsize.x, glyph.texsize.y);
		character. channel = glyph.channel;
		character.   color = color;

		prevCode = glyph.code;
	}

	return count;
}

void Font::renderInstanced(Context const &context, int32_t count) const
{
	LUCID_VALIDATE(_mesh, "use of uninitialized font");
	_mesh->renderInstanced(context, count);
}

void Font::drawInstanced(int32_t count) const
{
	LUCID_VALIDATE(_mesh, "use of uninitialized font");
	_mesh->drawInstanced(count);
}

void Font::initialize(LUCID_CORE::Unserializer &reader)
{
	shutdown();

	_name = reader.read<std::string>();
	_defaultSpacing = reader.read<float32_t>();

	if (reader.read<bool>())
		_mesh = Resources::get<Mesh>(reader.read<std::string>());
	else
		_mesh.reset(Mesh::create(reader));

	int32_t size = reader.read<int32_t>();
	LUCID_VALIDATE(0 == (size % sizeof(Glyph)), "");

	int32_t count = size / sizeof(Glyph);
	for (int32_t i = 0; i < count; ++i)
	{
		Glyph glyph = reader.read<Glyph>();

		LUCID_VALIDATE(_glyphs.end() == _glyphs.find(glyph.code), "duplicate glyph code specified for font, " + _name + ", specified");
		_glyphs.insert(std::make_pair(glyph.code, glyph));
	}

	size = reader.read<int32_t>();
	LUCID_VALIDATE(0 == (size % 2), "");

	count = size / 2;
	for (int32_t i = 0; i < count; ++i)
	{
		Key key;
		key.first = reader.read<uint8_t>();
		key.second = reader.read<uint8_t>();

		LUCID_VALIDATE(_spacing.end() == _spacing.find(key), "duplicate glyph spacing specified for font, " + _name + ", specified");

		_spacing.insert(std::make_pair(key, reader.read<float32_t>()));
	}
}

void Font::shutdown()
{
	_spacing.clear();
	_glyphs.clear();

	_mesh.reset();

	_defaultSpacing = 0.f;

	_name.clear();
}

LUCID_GIGL_END
