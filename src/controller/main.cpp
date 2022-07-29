#include "controller/main.h"
#include "input/input.h"
#include "app/definitions.h"
#include "env/env.h"

#include <tools/json.h>
#include <tools/file_utils.h>
#include <ldv/ttf_representation.h>
#include <ldv/bitmap_representation.h>
#include <sstream>
#include <numeric>
#include <map>

using namespace controller;

main::main(
	lm::logger& _log,
	const ldtools::ttf_manager& _tff_manager,
	tools::message_manager& _message_manager,
	const tools::ticker& _ticker,
	animation_editor::animations& _animations,
	const animation_editor::visuals& _visuals,
	const env::env_interface& _env,
	ldv::rect _display_rect,
	unsigned int _margin,
	unsigned int _height
):
	log{_log},
	ttf_manager{_tff_manager},
	message_manager{_message_manager},
	ticker{_ticker},
	animations{_animations},
	visuals{_visuals},
	display_rect{_display_rect},
	animation_list{_display_rect.h - _margin, _height},
	margin_top_list{_margin},
	h_list_item{_height} {

	//Subscribe to message manager changes...
	message_manager.subscribe("main", std::bind(&main::receive_message, this, std::placeholders::_1));

	//Mount layout...
	layout.map_font(
		animation_editor::definitions::main_font_name,
		ttf_manager.get(animation_editor::definitions::main_font_name, animation_editor::definitions::main_font_size)
	);

	auto root=tools::parse_json_string(
		tools::dump_file(
		_env.build_data_path("layouts.json")
		)
	);

	layout.parse(root["main"]);
}

void main::awake(
	dfw::input& /*input*/
) {
	if(intra_update_on_awake) {

		const auto index=animation_list.get_current_index();

		update_hud();
		update_list();

		if(intra_keep_index_on_awake) {

			animation_list.set_index(index);
		}

	}

	intra_file_browser_allow_new=false;
	intra_update_on_awake=true;
	intra_keep_index_on_awake=true;
}

void main::slumber(
	dfw::input& /*input*/
) {

	//TODO:
}

void main::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& /*lid*/
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::f1)) {

		push_state(state_help);
		return;
	}

	if(_input.is_input_down(input::load)) {

		intra_file_browser_allow_new=false;
		push_state(state_file_browser);
		return;
	}

	if(_input.is_input_down(input::save)) {

		if(_input.is_input_pressed(input::lctrl)) {

			intra_file_browser_allow_new=true;
			push_state(state_file_browser);
			return;
		}
		else {

			//This simbolizes the desire to save in the state_driver.
			push_state(state_main);
			return;
		}
	}

	if(_input.is_input_down(input::change_id)) {

		change_animation_id();
		return;
	}

	if(_input.is_input_down(input::rename)) {

		rename_animation();
		return;
	}

	if(_input.is_input_down(input::enter)) {

		enter_animation_frames();
		return;
	}

	if(_input.is_input_down(input::pageup)) {

		move_animation_up();
		return;
	}

	if(_input.is_input_down(input::pagedown)) {

		move_animation_down();
		return;
	}

	if(_input.is_input_down(input::del)) {

		erase_animation();
		return;
	}

	if(_input.is_input_down(input::insert)) {

		insert_animation();
		return;
	}

	if(_input.is_input_down(input::down)) {

		animation_list.next();
		update_hud();
		return;
	}
	else if(_input.is_input_up(input::up)) {

		animation_list.previous();
		update_hud();
		return;
	}
}

void main::draw(
	ldv::screen& _screen,
	int /*fps*/
) {

	layout.draw(_screen);

	for(const auto& item : animation_list.get_page()) {

		std::stringstream ss;

		ss<<item.item.animation.name
			<<" ["<<item.item.animation.id
			<<", "<<item.item.duration<<"ms, "
			<<item.item.animation.frames.size()<<" frames]";

		auto color=std::find(
			std::begin(repeated_ids),
			std::end(repeated_ids),
			item.item.animation.id
		)==std::end(repeated_ids)
			? ldv::rgba8(255, 255, 255, 128)
			: ldv::rgba8(255, 0, 0, 128);

		if(animation_list.get_current_index()==item.index) {

			color.a=255;
		}

		ldv::ttf_representation txt(
			ttf_manager.get(animation_editor::definitions::main_font_name, animation_editor::definitions::main_font_size),
			color,
			""
		);

		const int max_w{ (display_rect.w / 4) * 3};
		int y=item.y+margin_top_list;

		txt.set_max_width(max_w);
		txt.set_text(ss.str());
		txt.go_to({0, y});
		txt.draw(_screen);

		if(!item.item.animation.frames.size()) {

			continue;
		}
		//Now for the animation...
		ldv::bitmap_representation bmp(
			visuals.get_texture(),
			{max_w, y, 32, 32},
			visuals.rect_for_animation_time(ticker.get(), item.item.animation, item.item.duration)
		);

		bmp.draw(_screen);
	}
}

void main::update_hud() {

	std::stringstream ss;
	ss<<(animation_list.get_current_index()+1)<<" / "<<animations.size()<<", frames in sheet: "<<visuals.get_table().size();

	ldv::ttf_representation& hud_rep=static_cast<ldv::ttf_representation&>(*layout.get_by_id("hud"));
	hud_rep.set_text(ss.str());

	hud_rep.align(
		display_rect,
		{
			ldv::representation_alignment::h::inner_left,
			ldv::representation_alignment::v::inner_top,
			6,
			6
		}
	);
}

void main::update_list() {

	std::map<std::size_t, std::size_t> id_count;

	animation_list.clear();
	for(const auto& anim : animations) {

		int duration=std::accumulate(
			std::begin(anim.frames),
			std::end(anim.frames),
			0,
			[](int _carry, const animation_editor::frame& _frame) {

				return _carry+_frame.duration_ms;
			}
		);

		if(!id_count.count(anim.id)) {
			id_count[anim.id]=0;
		}
		else {
			++id_count[anim.id];
		}

		animation_list.insert({
			anim,
			duration
		});
	}

	//The duplicated ids are those that in id_count are larger than 0.
	repeated_ids.clear();
	for(const auto& pair : id_count) {
		if(pair.second) {
			repeated_ids.push_back(pair.first);
		}
	}
}

void main::receive_message(
	tools::message_manager::notify_event_type /*_type*/
) {

	ldv::ttf_representation& msg_rep=static_cast<ldv::ttf_representation&>(*layout.get_by_id("messages"));

	if(!message_manager.size()) {

		msg_rep.set_text("");
		return;
	}

	msg_rep.set_text(message_manager.last());
	msg_rep.align(
		display_rect,
		{
			ldv::representation_alignment::h::center,
			ldv::representation_alignment::v::inner_bottom,
			0,
			6
		}
	);
}

void main::move_animation_up() {

	if(animation_list.size() < 2) {

		return;
	}

	const auto index=animation_list.get_current_index();
	if(!index) {

		return;
	}

	std::swap(animations[index], animations[index-1]);

	update_list();

	animation_list.set_index(index);
	animation_list.previous();

	update_hud();
}

void main::move_animation_down() {

	if(animation_list.size() < 2) {

		return;
	}

	const auto index=animation_list.get_current_index();
	if(index==animation_list.size()-1) {

		return;
	}

	std::swap(animations[index], animations[index+1]);

	update_list();

	animation_list.set_index(index);
	animation_list.next();

	update_hud();
}

void main::erase_animation() {

	if(!animation_list.size()) {

		return;
	}

	const auto index=animation_list.get_current_index();
	animations.erase(std::begin(animations)+index);

	update_list();

	animation_list.set_index(index);
	if(index >= animations.size()) {
		animation_list.previous();
	}

	update_hud();

	message_manager.add("animation deleted");
}

void main::rename_animation() {

	if(!animations.size()) {

		return;
	}

	push_state(state_rename_animation);
}

void main::change_animation_id() {

	if(!animations.size()) {

		return;
	}

	push_state(state_change_animation_id);
}

void main::enter_animation_frames() {

	if(!animations.size()) {

		return;
	}

	push_state(state_animation);
}


void main::insert_animation() {

	const auto index=animation_list.get_current_index();
	animations.insert(std::begin(animations)+index, {0, "new animation", {}});

	update_list();

	animation_list.set_index(index);

	update_hud();

	message_manager.add("emtpy animation created");
}
