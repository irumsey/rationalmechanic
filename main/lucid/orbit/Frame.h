#pragma once

#include <string>

///
///
///

namespace lucid {
namespace orbit {

	///
	///
	///
	class Frame
	{
	public:
		Frame(std::string const &label);

		Frame(Frame *parent, std::string const &label);

		virtual ~Frame() = default;

		Frame *parent() const;

		std::string const &label() const;

	private:
		Frame *_parent = nullptr;
		std::string _label;

	};

	inline Frame *Frame::parent() const
	{
		return _parent;
	}

	inline std::string const &Frame::label() const
	{
		return _label;
	}

}	///	orbit
}	///	lucid
