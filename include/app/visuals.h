#pragma once

#include <ldv/texture.h>
#include <ldtools/sprite_table.h>
#include <ldtools/time_definitions.h>
#include "app/animation.h"
#include <memory>

namespace animation_editor {

//!Lifetime manager for the texture and tileset.
class visuals {

	public:

	ldv::texture&                   get_texture() const {return *texture;}
	const ldtools::sprite_table&    get_table() const {return table;}
	ldv::rect                       rect_for_animation_time(ldtools::tdelta _current_time, const animation_editor::animation& _animation, int _duration) const;
/**
 * produces the flip flags that result from combining the sprite and
 * animation frame flags.*/
	int                             flip_flags_for_animation_time(ldtools::tdelta _current_time, const animation_editor::animation& _animation, int _duration) const;

	void                            load_texture(const std::string&);
	void                            load_table(const std::string&);

	private:

	std::unique_ptr<ldv::texture>   texture{nullptr};
	ldtools::sprite_table           table;
};
}
