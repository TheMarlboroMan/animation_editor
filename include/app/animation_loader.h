#pragma once

#include "app/animation.h"

#include <string>

namespace animation_editor {

class animation_loader {

	public:

	animations          from_string(const std::string&);
	animations          from_file(const std::string&);

	private:
};

}
