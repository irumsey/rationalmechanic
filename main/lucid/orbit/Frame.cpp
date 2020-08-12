#include "Frame.h"

namespace lucid {
namespace orbit {

	Frame::Frame(std::string const &label)
		: _label(label)
	{
	}

	Frame::Frame(Frame *parent, std::string const &label)
		: _parent(parent)
		, _label(label)
	{
	}

}	///	orbit
}	///	lucid
