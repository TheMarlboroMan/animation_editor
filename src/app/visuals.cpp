#include "app/visuals.h"

#include <ldv/sdl_video_tools.h>

using namespace animation_editor;

void visuals::load_texture(
	const std::string& _filepath
) {

	ldv::surface surface(ldv::load_image(_filepath));
	texture.reset(new ldv::texture(surface));
}

void visuals::load_table(
	const std::string& _filepath
) {

	table.load(_filepath);
}
