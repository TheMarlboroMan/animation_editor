#pragma once

#include "app/frame.h"

#include <string>
#include <vector>

namespace animation_editor {

struct animation {

	std::size_t             id;
	std::string             name;
	std::vector<frame>      frames;
};

using animations=std::vector<animation>;

}
