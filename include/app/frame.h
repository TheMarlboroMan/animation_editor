#pragma once

#include <cstddef>

namespace animation_editor {

struct frame {

	std::size_t         index;  //!< Frame index.
	int                 duration_ms;
};

}
