#include "controller/main.h"
#include "input/input.h"
#include "app/definitions.h"

#include <tools/json.h>
#include <tools/file_utils.h>
#include <ldv/ttf_representation.h>
#include <sstream>
#include <numeric>

using namespace controller;

main::main(
	lm::logger& _log,
	const ldtools::ttf_manager& _tff_manager,
	tools::message_manager& _message_manager,
	const tools::ticker& _ticker,
	animation_editor::animations& _animations,
	const animation_editor::visuals& _visuals,
	ldv::rect _display_rect,
	int _margin,
	int _height
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
			"resources/layouts.json"
		)
	);

	layout.parse(root["main"]);
}

void main::awake(
	dfw::input& /*input*/
) {
	if(intra_update_on_awake) {

		update_hud();
		update_list();
		intra_update_on_awake=false;
	}
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

		int duration=std::accumulate(
			std::begin(item.item.frames),
			std::end(item.item.frames),
			0,
			[](int _carry, const animation_editor::frame& _frame) {

				return _carry+_frame.duration_ms;
			}
		);

		ss<<item.item.name<<" ["<<item.item.id<<", "<<duration<<"ms]";

		ldv::ttf_representation txt(
			ttf_manager.get(animation_editor::definitions::main_font_name, animation_editor::definitions::main_font_size),
			animation_list.get_current_index()==item.index
				? ldv::rgba8(255, 255, 255, 255)
				: ldv::rgba8(255, 255, 255, 128),
			ss.str()
		);

		txt.go_to({0, item.y+margin_top_list});
		txt.draw(_screen);
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

	animation_list.clear();
	for(const auto& anim : animations) {

		animation_list.insert(anim);
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
}

void main::insert_animation() {

	const auto index=animation_list.get_current_index();
	animations.insert(std::begin(animations)+index, {0, "new animation", {}});

	update_list();

	animation_list.set_index(index);

	update_hud();
}
