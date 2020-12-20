#include "controller/rename_animation.h"
#include "input/input.h"
#include "app/definitions.h"

#include <ldv/ttf_representation.h>
#include <ldv/box_representation.h>

using namespace controller;

rename_animation::rename_animation(
	lm::logger& _log,
	ldtools::ttf_manager& _ttf_manager
)
	:log{_log},
	ttf_manager{_ttf_manager} {

}

void rename_animation::awake(
	dfw::input& _input
) {

	if(nullptr==animation) {

		throw std::runtime_error("animation cannot be null for rename animation controller");
	}

	current_value=animation->name;
	_input().start_text_input();
/*
	_input().set_text_filter([](const SDL_Event& _event) -> bool {
		try {
			int val=std::stoi(_event.text.text);
			return true;
		}
		catch(std::invalid_argument&) {
			return false;
		}
	});
*/
}

void rename_animation::slumber(
	dfw::input& _input
) {

	animation=nullptr;
	_input().stop_text_input();
	_input().clear_text_filter();
}

void rename_animation::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

 	if(_input.is_input_down(input::escape)) {

		pop_state();
		return;
	}

	if(_input.is_input_down(input::enter)) {

		animation->name=current_value;
		pop_state();
		return;
	}

	if(_input.is_input_down(input::backspace)) {

		if(current_value.length()) {
			current_value.pop_back();
		}

		_input().clear_text_input();
		return;
	}

	if(_input().is_text_input()) {

		current_value+=_input().get_text_input();
		_input().clear_text_input();
	}
}

void rename_animation::request_draw(
	dfw::controller_view_manager& _cvm
) {

	_cvm.add(state_main); 
	_cvm.add_ptr(this);
}

void rename_animation::draw(
	ldv::screen& _screen, 
	int /*fps*/
) {

	ldv::box_representation bg{
		_screen.get_rect(),
		ldv::rgba8(0,0,0,64)
	};
	bg.set_blend(ldv::representation::blends::alpha);
	bg.draw(_screen);

	std::string contents{"animation name:\n"};
	contents+=current_value;

	ldv::ttf_representation txt(
		ttf_manager.get(animation_editor::definitions::main_font_name, animation_editor::definitions::main_font_size),
		ldv::rgba8(255, 255, 255, 255),
		contents
	);

	auto rec=txt.get_view_position();
	rec.w+=20;
	rec.h+=20;

	ldv::box_representation box{
		rec,
		ldv::rgba8(0,0,0,255)
	};

	ldv::representation_alignment center{
		ldv::representation_alignment::h::center,
		ldv::representation_alignment::v::center
	};

	auto align_and_draw=[&_screen, center](ldv::representation& _rep) {

		_rep.align(
			_screen.get_rect(),
			center
		);
		_rep.draw(_screen);
	};

	align_and_draw(box);

	box.set_color(ldv::rgba8(255, 255, 255, 255));
	box.set_filltype(ldv::polygon_representation::type::line);

	align_and_draw(box);
	align_and_draw(txt);
}
