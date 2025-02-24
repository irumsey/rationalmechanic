#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/core/Identity.h>
#include <lucid/core/Logger.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Mesh.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

LUCID_GIGL_BEGIN

class RenderContext;

/// Font
/// 
///	TBD: change such that the glyph code/index is larger (more flexible)
///      for now, just ascii
class Font
{
public:
	/// 
	/// 
	/// 
	enum ALIGNMENT
	{
		ALIGN_LEFT = 0,
		ALIGN_CENTER,
		ALIGN_RIGHT,
	};

	/// 
	/// 
	/// 
#	pragma pack(push)
#	pragma pack(8)
	struct Character
	{
		LUCID_GAL::Vector4 position;	// position and size
		LUCID_GAL::Vector4 texcoord;	// texcoord and texsize
		LUCID_GAL::Color    channel;	// the color channel glyph resides
		LUCID_GAL::Color      color;	// color of character instance
	};
#	pragma pack(pop)

	Font() = delete;

	Font(LUCID_CORE::Unserializer &reader);

	virtual ~Font();

	LUCID_CORE::Identity const &identity() const;

	std::string const &name() const;

	/// Note: caller ensures buffer is large enough for the text specified
	size_t typeset(Character *buffer, ALIGNMENT align, LUCID_GAL::Vector2 const &position, float32_t size, std::string const &text, LUCID_GAL::Color const &color);

	///	this assumes the per-instance stream(s) are already set.
	void renderInstanced(Context const &context, int32_t count) const;

	///	this assumes the per-instance stream(s) are already set.
	void drawInstanced(int32_t count) const;

	static Font *create(std::string const &path);

	static Font *create(LUCID_CORE::Unserializer &reader);

private:
	/// 
	/// 
	/// 
#	pragma pack(push)
#	pragma pack(1)						// reading binary data from file
	struct Glyph
	{
		uint8_t code = 0;				// glyph's code

		LUCID_GAL::Vector2 texcoord;	// location in texture
		LUCID_GAL::Vector2  texsize;	// width and height, in pixels, of glyph in texture
		LUCID_GAL::Color    channel;	// used to select the proper color channel (ex: dot(texel, channel) -> glyph pixel)
	};
#pragma pack(pop)

	LUCID_CORE::Identity _id;
	std::string _name;

	std::shared_ptr<LUCID_GIGL::Mesh> _mesh;

	float32_t _defaultSpacing = 0.f;

	std::unordered_map<uint8_t, Glyph> _glyphs;

	typedef std::pair<uint8_t, uint8_t> Key;

	struct Hash
	{
		size_t operator()(Key const &key) const
		{
			return ((key.first << 8) | key.second);
		}
	};

	std::unordered_map<Key, float32_t, Hash> _spacing;

	void initialize(LUCID_CORE::Unserializer &reader);

	void shutdown();

	Glyph const &lookup(uint8_t code) const;

	float32_t spacing(uint8_t prevCode, uint8_t currCode) const;

};

inline LUCID_CORE::Identity const &Font::identity() const
{
	return _id;
}

inline std::string const &Font::name() const
{
	return _name;
}

inline Font::Glyph const &Font::lookup(uint8_t code) const
{
	// sample texture at (0,0) with size (0,0) and with no channels
	// specified will result in a blank character on screen...
	static Glyph const blank = { 0, { 0, 0}, { 0, 0}, { 0, 0, 0, 0 } };

	auto const iter = _glyphs.find(code);
	if (iter != _glyphs.end())
		return iter->second;

	std::ostringstream os;
	os << "invalid glyph code, " << code << ", specified in text using font, " << _name;

	LUCID_CORE::log("WARN", os.str());
	return blank;
}

inline float32_t Font::spacing(uint8_t prevCode, uint8_t currCode) const
{
	auto const iter = _spacing.find(Key(prevCode, currCode));
	return ((iter == _spacing.end()) ? _defaultSpacing : iter->second);
}

LUCID_GIGL_END
