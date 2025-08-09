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

ldv::rect visuals::rect_for_animation_time(
	ldtools::tdelta _current_time,
	const animation_editor::animation& _animation,
	int _duration
) const {

	const auto& frames=_animation.frames;
	const auto& anim_table=get_table();

	if(frames.size()==1) {

		return anim_table.get(frames.at(0).index).box;
	}
	else {

		ldtools::tdelta duration_seconds=_duration / 1000.;
		ldtools::tdelta current_time=fmod(_current_time, duration_seconds);
		ldtools::tdelta framesum{0.};

		for(const auto& frame : frames) {

			ldtools::tdelta frame_duration=frame.duration_ms / 1000.;

			if(framesum <= current_time && current_time < framesum+frame_duration) {

				return anim_table.get(frame.index).box;
			}
			framesum+=frame_duration;
		}
	}

	//Stupid default that should never happen.
	return anim_table.get(frames.at(0).index).box;
}

int visuals::flip_flags_for_animation_time(
	ldtools::tdelta _current_time,
	const animation_editor::animation& _animation,
	int _duration
) const {

	const auto& frames=_animation.frames;
	const auto& anim_table=get_table();

	int flags=0;
	ldtools::tdelta duration_seconds=_duration / 1000.;
	ldtools::tdelta current_time=fmod(_current_time, duration_seconds);
	ldtools::tdelta framesum{0.};

	for(const auto& frame : frames) {

		ldtools::tdelta frame_duration=frame.duration_ms / 1000.;

		if(framesum <= current_time && current_time < framesum+frame_duration) {

			const auto& sprite=anim_table.get(frame.index);

			bool flipped_horizontally=frame.flipped_horizontal xor sprite.is_flipped_horizontally();
			bool flipped_vertically=frame.flipped_vertical xor sprite.is_flipped_vertically();
			int degrees=(frame.rotation_degrees + sprite.get_rotation()) % 360;

			if(flipped_horizontally) flags+=1;
			if(flipped_vertically) flags+=2;
			switch(degrees) {

				case 90: flags+=4; break;
				case 180: flags+=8; break;
				case 270: flags+=12; break;
			}
			return flags;
		}

		framesum+=frame_duration;
	}

	throw std::runtime_error("error: found end of animation and could not fetch frame data");
}

