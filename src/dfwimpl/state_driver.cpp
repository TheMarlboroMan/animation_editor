#include "dfwimpl/state_driver.h"
#include "input/input.h"
#include "controller/states.h"
#include "app/definitions.h"
#include "app/animation_loader.h"
#include "app/animation_saver.h"


#include <lm/log.h>
#include <tools/string_utils.h>

#include <algorithm>

using namespace dfwimpl;

state_driver::state_driver(
	dfw::kernel& kernel,
	dfwimpl::config& c,
	const env::env_interface& _env
)
	:state_driver_interface(controller::t_states::state_main),
	config(c),
	log(kernel.get_log()),
	env{_env},
	message_manager(30) {

	lm::log(log).info()<<"setting state check function..."<<std::endl;

	states.set_function([](int v){
		return v > controller::state_min && v < controller::state_max;
	});

	lm::log(log).info()<<"init state driver building: preparing video..."<<std::endl;
	prepare_video(kernel);

	lm::log(log).info()<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	lm::log(log).info()<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	lm::log(log).info()<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);

	lm::log(log).info()<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	lm::log(log).info()<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input());

	lm::log(log).info()<<"state driver will check input parameters and start up"<<std::endl;

	start_application(kernel);
	lm::log(log).info()<<"state driver fully constructed"<<std::endl;

}

void state_driver::init(dfw::kernel&) {

}


void state_driver::prepare_video(dfw::kernel& kernel) {

	const auto argman=kernel.get_arg_manager();

	int w=config.int_from_path("video:window_w_px"),
	    h=config.int_from_path("video:window_h_px");

	if(argman.exists("-w") && argman.arg_follows("-w")) {

		const std::string& window_size_str=argman.get_following("-w");

		auto xpos=window_size_str.find("x");
		if(std::string::npos==xpos) {
			throw std::runtime_error("-w parameter must be specified in wxh");
		}

		w=std::stoi(window_size_str.substr(0, xpos));
		h=std::stoi(window_size_str.substr(xpos+1));

		lm::log(log).info()<<"window size specified by command line as "<<window_size_str<<" ["<<w<<"x"<<h<<"]"<<std::endl;
	}

	std::stringstream ss;
	ss<<"Animation Editor v"<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"-"<<BUILD_VERSION;

	kernel.init_video_system({
		w,
		h,
		w,
		h,
		ss.str(),
		false,
		config.get_screen_vsync()
	});

	auto& screen=kernel.get_screen();
	screen.set_fullscreen(config.bool_from_path("video:fullscreen"));

	ttf_manager.insert(
		animation_editor::definitions::main_font_name,
		animation_editor::definitions::main_font_size,
		env.build_data_path(
			"assets/fonts/BebasNeue-Regular.ttf"
		)
	);
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system({
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	});
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	using namespace dfw;

	std::vector<input_pair> pairs{
		{{input_description::types::keyboard, SDL_SCANCODE_ESCAPE, 0}, input::escape}
	};

	auto add=[&](std::string _token, int _input_type) {

		for(const auto desc : dfw::input_description_from_config_token(config.token_from_path(_token))) {

			pairs.push_back({desc, _input_type});
		}
	};

	add("input:left", input::left);
	add("input:right", input::right);
	add("input:up", input::up);
	add("input:down", input::down);
	add("input:pageup", input::pageup);
	add("input:pagedown", input::pagedown);
	add("input:enter", input::enter);
	add("input:save", input::save);
	add("input:load", input::load);
	add("input:lctrl", input::lctrl);
	add("input:lshift", input::lshift);
	add("input:del", input::del);
	add("input:insert", input::insert);
	add("input:rename", input::rename);
	add("input:change_id", input::change_id);
	add("input:backspace", input::backspace);
	add("input:plus", input::plus);
	add("input:minus", input::minus);
	add("input:f1", input::f1);
	add("input:flip", input::flip);

	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(dfw::kernel& /*kernel*/) {

/*
	dfw::resource_loader r_loader(kernel.get_video_resource_manager(), kernel.get_audio_resource_manager());

	r_loader.generate_textures(tools::explode_lines_from_file(std::string("data/resources/textures.txt")));
	r_loader.generate_sounds(tools::explode_lines_from_file(std::string("data/resources/audio.txt")));
	r_loader.generate_music(tools::explode_lines_from_file(std::string("data/resources/music.txt")));
*/
}

void state_driver::register_controllers(
	dfw::kernel& _kernel
) {

	auto reg=[this](ptr_controller& _ptr, int _i, dfw::controller_interface * _ci) {
		_ptr.reset(_ci);
		register_controller(_i, *_ptr);
	};

	reg(
		c_main,
		controller::t_states::state_main,
		new controller::main(
			log,
			ttf_manager,
			message_manager,
			ticker,
			animations,
			visuals,
			env,
			_kernel.get_screen().get_rect(),
			config.int_from_path("app:margin_top_list"),
			config.int_from_path("app:h_list_item")
		)
	);
	reg(
		c_help,
		controller::t_states::state_help,
		new controller::help(
			log,
			ttf_manager,
			env,
			_kernel.get_screen().get_w(),
			_kernel.get_screen().get_h()
		)
	);
	reg(
		c_file_browser,
		controller::t_states::state_file_browser,
		new controller::file_browser(
			log,
			ttf_manager,
			env,
			_kernel.get_screen().get_w()
		)
	);
	reg(
		c_rename_animation,
		controller::t_states::state_rename_animation,
		new controller::rename_animation(
			log,
			ttf_manager
		)
	);
	reg(
		c_change_animation_id,
		controller::t_states::state_change_animation_id,
		new controller::change_animation_id(
			log,
			ttf_manager
		)
	);
	reg(
		c_animation,
		controller::t_states::state_animation,
		new controller::animation(
			log,
			ttf_manager,
			message_manager,
			ticker,
			visuals,
			env,
			_kernel.get_screen().get_rect(),
			config.int_from_path("app:margin_top_list"),
			config.int_from_path("app:h_list_item")
		)
	);
	reg(
		c_frame_selection,
		controller::t_states::state_frame_selection,
		new controller::frame_selection(
			log,
			ttf_manager,
			visuals,
			_kernel.get_screen().get_rect(),
			config.int_from_path("app:margin_list_grid"),
			config.int_from_path("app:size_list_grid")
		)
	);
	//[new-controller-mark]
}

void state_driver::prepare_state(
	int _next,
	int _current
) {

	//This looks like a switchboard :D.

	controller::file_browser * file_browser=static_cast<controller::file_browser *>(c_file_browser.get());
	controller::main * main=static_cast<controller::main *>(c_main.get());
	controller::rename_animation * rename=static_cast<controller::rename_animation *>(c_rename_animation.get());
	controller::change_animation_id * change_id=static_cast<controller::change_animation_id *>(c_change_animation_id.get());
	controller::animation * animation_controller=static_cast<controller::animation *>(c_animation.get());
	controller::frame_selection * frame_selection=static_cast<controller::frame_selection *>(c_frame_selection.get());

	using namespace controller;
	switch(_next) {

		case t_states::state_animation:

			switch(_current) {

				case t_states::state_main:
						animation_controller->intra_set_animation(
						animations.at(main->intra_get_current_index())
					);
				break;
				case t_states::state_frame_selection:

					if(frame_selection->intra_get_success()) {

						animation_controller->intra_set_frame_index(frame_selection->intra_get_frame_id());
					}
				break;
			}
		break;

		case t_states::state_rename_animation:

			rename->intra_set_animation(
				&animations.at(main->intra_get_current_index())
			);

		break;

		case t_states::state_change_animation_id:

			change_id->intra_set_animation(
				&animations.at(main->intra_get_current_index())
			);

		break;

		case t_states::state_file_browser:

			switch(_current) {

				case t_states::state_main:
					file_browser->intra_set_allow_create(main->intra_get_file_browser_allow_new());
				break;
			}

		break;
		case t_states::state_main:

			switch(_current) {

				case t_states::state_file_browser:
					if(!file_browser->intra_get_success()) {

						return;
					}

					//Save into new/same file.
					if(main->intra_get_file_browser_allow_new()) {

						save(file_browser->intra_get_result());
					}
					//Load from file.
					else {

						if(load(file_browser->intra_get_result())) {
							main->intra_set_update_on_awake(true);
							main->intra_set_keep_index_on_awake(false);
						}
					}
				break;

				case t_states::state_animation:
				case t_states::state_rename_animation:
				case t_states::state_change_animation_id:
					main->intra_set_update_on_awake(true);
					main->intra_set_keep_index_on_awake(true);
				break;

				case t_states::state_main:

					if(!current_filename.size()) {

						message_manager.add("no session open, use lctrl+s");
					}
					else {
						save(current_filename);
					}
				break;
			}
		break;
		default:
		break;
	}
}

void state_driver::common_pre_loop_input(dfw::input& input, float /*delta*/) {

	if(input().is_event_joystick_connected()) {
		lm::log(log).info()<<"New joystick detected..."<<std::endl;
		virtualize_input(input);
	}
}

void state_driver::common_loop_input(
	dfw::input& /*input*/,
	float _delta
) {

	ticker.tick(_delta);
	message_manager.tick(_delta);
}

void state_driver::common_pre_loop_step(float /*delta*/) {

}

void state_driver::common_loop_step(float /*delta*/) {

}

void state_driver::virtualize_input(dfw::input& input) {

	lm::log(log).info()<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {
		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, 15000);
		lm::log(log).info()<<"Joystick virtualized "<<i<<std::endl;
	}
}

void state_driver::start_application(
	dfw::kernel& _kernel
) {
	const auto argman=_kernel.get_arg_manager();

	if(argman.exists("-i") && argman.arg_follows("-i")) {

		visuals.load_texture(argman.get_following("-i"));
	}
	else {

		throw std::runtime_error("must use -i parameter to load the sprite sheet image");
	}

	if(argman.exists("-s") && argman.arg_follows("-s")) {

		visuals.load_table(argman.get_following("-s"));
	}
	else {

		throw std::runtime_error("must use -s parameter to load the sprite sheet table");
	}

	if(argman.exists("-f") && argman.arg_follows("-f")) {

		load(argman.get_following("-f"));
	}
	else {

		message_manager.add("ready, press f1 for help");
	}
}

void state_driver::save(
	const std::string& _filename
) {

	try {
		animation_editor::animation_saver saver;
		saver.to_file(_filename, animations);
		current_filename=_filename;
		message_manager.add(std::string{"saved to "}+current_filename);
	}
	catch(std::exception& e) {

		message_manager.add(std::string{"save failed: "}+e.what());
	}
}

bool state_driver::load(
	const std::string& _filename
) {

	try {

		animation_editor::animation_loader loader;
		auto loaded=loader.from_file(_filename);
		std::swap(loaded, animations);
		current_filename=_filename;
		message_manager.add(std::string{"loaded from "}+current_filename);
		return true;
	}
	catch(std::exception& e) {

		message_manager.add(std::string{"loading failed: "}+e.what());
		return false;
	}
}
