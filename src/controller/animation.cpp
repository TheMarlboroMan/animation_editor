#include "controller/animation.h"
#include "input/input.h"
#include "app/definitions.h"

#include <tools/json.h>
#include <tools/file_utils.h>
#include <ldv/ttf_representation.h>
#include <ldv/bitmap_representation.h>

#include <sstream>
#include <numeric>

using namespace controller;

animation::animation(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager,
	tools::message_manager& _message_manager,
	const tools::ticker& _ticker,
	const animation_editor::visuals& _visuals,
	const env::env_interface& _env,
	ldv::rect _display,
	unsigned int _margin,
	unsigned int _height
):
	log{_log},
	ttf_manager{_ttf_manager},
	message_manager{_message_manager},
	ticker{_ticker},
	visuals{_visuals},
	display_rect{_display},
	frame_list{display_rect.h - _margin, _height},
	margin_top_list{_margin},
	h_list_item{_height}{

	//Subscribe to message manager changes...
	message_manager.subscribe("animation", std::bind(&animation::receive_message, this, std::placeholders::_1));

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

	layout.parse(root["main"]); //Use the very same layout as main...
}

void animation::awake(
	dfw::input& /*input*/
) {

	auto index=frame_list.get_current_index();

	update_hud();
	update_list();

	if(preserve_index_on_enter) {

		frame_list.set_index(index);
		preserve_index_on_enter=false;
	}
}

void animation::slumber(
	dfw::input& /*input*/
) {

	if(!preserve_animation_on_exit) {

		current_animation=nullptr;
	}

	preserve_animation_on_exit=false;
}

void animation::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& /*lid*/
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		pop_state();
	}

	if(_input.is_input_down(input::enter)) {

		enter_animation_frames();
		return;
	}

	bool lctrl=_input.is_input_pressed(input::lctrl);
	bool lshift=_input.is_input_pressed(input::lshift);

	if(
		_input.is_input_down(input::pageup)
		|| (lctrl && _input.is_input_down(input::up))
	) {

		move_frame_up();
		return;
	}

	if(
		_input.is_input_down(input::pagedown)
		|| (lctrl && _input.is_input_down(input::down))
	) {

		move_frame_down();
		return;
	}

	if(_input.is_input_down(input::del)) {

		erase_frame();
		return;
	}

	if(_input.is_input_down(input::insert)) {

		insert_frame();
		return;
	}

	if(_input.is_input_down(input::down)) {

		lshift
			? frame_list.next_page()
			: frame_list.next();
		update_hud();
		return;
	}
	else if(_input.is_input_down(input::up)) {

		lshift
			? frame_list.previous_page()
			: frame_list.previous();
		update_hud();
		return;
	}
	
	if(_input.is_input_down(input::flip)) {

		flip_frame(lctrl);
	}

	if(_input.is_input_down(input::rename)) {

		rotate_frame(lctrl);
	}

	if(_input.is_input_down(input::plus)) {

		make_frame_longer(lctrl);
		return;
	}
	else if(_input.is_input_down(input::minus)) {

		make_frame_shorter(lctrl);
		return;
	}

}

void animation::draw(
	ldv::screen& _screen,
	int /*fps*/
) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));

	layout.draw(_screen);

	if(!current_animation->frames.size()) {

		return;
	}

	const int max_w{ ((int)display_rect.w / 4) * 3};

	for(const auto& item : frame_list.get_page()) {

		std::stringstream ss;

		ss<<item.item.index<<", "<<item.item.duration_ms<<"ms, ";

		auto color=frame_list.get_current_index()==item.index
			? ldv::rgba8(255, 255, 255, 255)
			: ldv::rgba8(255, 255, 255, 128);

		ldv::ttf_representation txt(
			ttf_manager.get(animation_editor::definitions::main_font_name, animation_editor::definitions::main_font_size),
			color,
			""
		);

		int y=item.y+margin_top_list;

		txt.set_max_width(max_w);
		txt.set_text(ss.str());
		txt.go_to({0, y});
		txt.draw(_screen);

		const auto& frame=visuals.get_table().get(item.item.index);

		ldv::bitmap_representation bmp(
			visuals.get_texture(),
			{max_w, y, 32, 32},
			frame.box
		);

		//Start with the sprite transformations and then add the frame ones...
		bool flipped_horizontal=frame.is_flipped_horizontally() xor item.item.flipped_horizontal;
		bool flipped_vertical=frame.is_flipped_vertically() xor item.item.flipped_vertical;
		int degrees=(frame.get_rotation()+item.item.rotation_degrees) % 360;

		bmp.set_invert_horizontal(flipped_horizontal);
		bmp.set_invert_vertical(flipped_vertical);
		bmp.center_rotation_center();
		bmp.set_rotation(degrees);

		bmp.draw(_screen);
	}

	//Now for the preview...
	ldv::bitmap_representation bmp(
		visuals.get_texture(),
		{max_w, 0, 32, 32}, //TODO: This blows. Should change with each frame.
		visuals.rect_for_animation_time(ticker.get(), *current_animation, duration)
	);

	int flipped_mask=visuals.flip_flags_for_animation_time(ticker.get(), *current_animation, duration);

	bmp.set_invert_horizontal(flipped_mask & 1);
	bmp.set_invert_vertical(flipped_mask & 2);

	int degrees=0;
	if(flipped_mask & 4) {

		degrees+=90;
	}

	if(flipped_mask & 8) {

		degrees+=180;
	}

	bmp.center_rotation_center();
	bmp.set_rotation(degrees);
	bmp.draw(_screen);
}

void animation::update_hud() {

	duration=std::accumulate(
		std::begin(current_animation->frames),
		std::end(current_animation->frames),
		0,
		[](int _carry, const animation_editor::frame& _frame) {

			return _carry+_frame.duration_ms;
		}
	);

	std::stringstream ss;
	ss<<(current_animation->name)
		<<" "<<(frame_list.get_current_index()+1)<<" / "<<current_animation->frames.size()
		<<", duration: "<<duration<<"ms"
		<<", frames in sheet: "<<visuals.get_table().size();

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

void animation::update_list() {

	frame_list.clear();
	for(const auto& frame : current_animation->frames) {

		frame_list.insert(frame);
	}
}

void animation::receive_message(
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

void animation::move_frame_up() {

	if(current_animation->frames.size() < 2) {

		return;
	}

	const auto index=frame_list.get_current_index();
	if(!index) {

		return;
	}

	std::swap(current_animation->frames[index], current_animation->frames[index-1]);

	update_list();

	frame_list.set_index(index);
	frame_list.previous();

	update_hud();
}

void animation::move_frame_down() {

	if(current_animation->frames.size() < 2) {

		return;
	}

	const auto index=frame_list.get_current_index();
	if(index==frame_list.size()-1) {

		return;
	}

	std::swap(current_animation->frames[index], current_animation->frames[index+1]);

	update_list();

	frame_list.set_index(index);
	frame_list.next();

	update_hud();
}

void animation::erase_frame() {

	if(!current_animation->frames.size()) {

		return;
	}

	const auto index=frame_list.get_current_index();
	current_animation->frames.erase(std::begin(current_animation->frames)+index);

	update_list();

	frame_list.set_index(index);
	if(index >= current_animation->frames.size()) {
		frame_list.previous();
	}

	update_hud();

	message_manager.add("frame deleted");
}

void animation::insert_frame() {

	const auto index=frame_list.get_current_index();
	current_animation->frames.insert(
		std::begin(current_animation->frames)+index,
		{
			visuals.get_table().begin()->first,
			100 //Default duration
		}
	);

	update_list();

	frame_list.set_index(index);

	update_hud();

	message_manager.add("empty frame created");
}

void animation::make_frame_longer(
	bool _fine
) {

	if(!(current_animation->frames.size())) {

		return;
	}

	const auto index=frame_list.get_current_index();
	auto& frame=current_animation->frames.at(index);
	frame.duration_ms+=_fine ? 1 : 10;

	update_list();
	frame_list.set_index(index);
	update_hud();
}

void animation::make_frame_shorter(
	bool _fine
) {

	if(!(current_animation->frames.size())) {

		return;
	}

	const auto index=frame_list.get_current_index();
	auto& frame=current_animation->frames.at(index);
	frame.duration_ms-=_fine ? 1 : 10;

	if(frame.duration_ms < 1) {

		frame.duration_ms=1;
	}

	update_list();
	frame_list.set_index(index);
	update_hud();
}

void animation::flip_frame(
	bool _vertical
) {

	if(!(current_animation->frames.size())) {

		return;
	}

	const auto index=frame_list.get_current_index();
	auto& frame=current_animation->frames.at(index);

	if(_vertical) {

		frame.flipped_vertical=!frame.flipped_vertical;
	}
	else {

		frame.flipped_horizontal=!frame.flipped_horizontal;
	}

	update_list();
	frame_list.set_index(index);
	update_hud();
}

void animation::rotate_frame(
	bool _counterclockwise
) {

	if(!(current_animation->frames.size())) {

		return;
	}

	const auto index=frame_list.get_current_index();
	auto& frame=current_animation->frames.at(index);

	int degrees=frame.rotation_degrees + (_counterclockwise ? -90 : 90);

	if(degrees < 0) {

		degrees=270;
	}
	else if(degrees==360) {

		degrees=0;
	}

	frame.rotation_degrees=degrees;

	update_list();
	frame_list.set_index(index);
	update_hud();
}

void animation::enter_animation_frames() {

	if(!(current_animation->frames.size())) {

		return;
	}

	preserve_animation_on_exit=true;
	preserve_index_on_enter=true;
	push_state(state_frame_selection);
}

void animation::intra_set_frame_index(std::size_t _index) {

	const auto index=frame_list.get_current_index();
	auto& frame=current_animation->frames.at(index);
	frame.index=_index;
}
