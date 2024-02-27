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
	float _current_time,
	const animation_editor::animation& _animation,
	int _duration
) const {

	const auto& frames=_animation.frames;
	const auto& anim_table=get_table();

	if(frames.size()==1) {

		return anim_table.get(frames.at(0).index).box;
	}
	else {

		float duration_seconds=_duration / 1000.f;
		float current_time=fmod(_current_time, duration_seconds);
		float framesum{0.f};

		for(const auto& frame : frames) {

			float frame_duration=frame.duration_ms / 1000.f;

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
	float _current_time,
	const animation_editor::animation& _animation,
	int _duration
) const {

	const auto& frames=_animation.frames;

	int flags=0;

	if(frames.size()==1) {

		const auto& fr=_animation.frames.at(0);
		if(fr.flipped_horizontal) flags+=1;
		if(fr.flipped_vertical) flags+=2;
		return flags;
	}
	else {

		float duration_seconds=_duration / 1000.f;
		float current_time=fmod(_current_time, duration_seconds);
		float framesum{0.f};

		for(const auto& frame : frames) {

			float frame_duration=frame.duration_ms / 1000.f;

			if(framesum <= current_time && current_time < framesum+frame_duration) {

				if(frame.flipped_horizontal) flags+=1;
				if(frame.flipped_vertical) flags+=2;
				return flags;
			}
			framesum+=frame_duration;
		}
	}

	//Stupid default that should never happen.
	return 0;
}

