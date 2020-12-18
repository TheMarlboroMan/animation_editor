#pragma once

#include "config.h"
#include "app/animation.h"
#include "app/visuals.h"
#include "tools/message_manager.h"
#include "tools/ticker.h"

#include <dfw/state_driver_interface.h>
#include <ldtools/ttf_manager.h>

#include <memory>

//Controllers.
#include "../../include/controller/main.h"
#include "../../include/controller/help.h"
#include "../../include/controller/file_browser.h"
//[new-controller-header-mark]

//Specific app_config
//#include "../app/placeholder.h"

namespace dfwimpl {

class state_driver:
	public dfw::state_driver_interface {

	public:
	                                state_driver(dfw::kernel& kernel, dfwimpl::config& config);

	virtual void                    common_pre_loop_input(dfw::input& input, float delta);
	virtual void                    common_pre_loop_step(float delta);
	virtual void                    common_loop_input(dfw::input& input, float delta);
	virtual void                    common_loop_step(float delta);
	virtual void                    prepare_state(int, int);
	virtual float                   get_max_timestep() const {return 0.03f;}

	private:

	void                            prepare_video(dfw::kernel&);
	void                            prepare_audio(dfw::kernel&);
	void                            prepare_input(dfw::kernel&);
	void                            prepare_resources(dfw::kernel&);
	void                            register_controllers(dfw::kernel&);
	void                            virtualize_input(dfw::input& input);
	void                            start_application(dfw::kernel&);

	void                            save(const std::string&);
	bool                            load(const std::string&);

	//references
	dfwimpl::config&                config;
	lm::logger&                     log;

	//properties
	ldtools::ttf_manager            ttf_manager;
	animation_editor::animations    animations;
	animation_editor::visuals       visuals;
	tools::message_manager          message_manager;
	tools::ticker                   ticker;
	std::string                     current_filename;

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller					c_main;
	ptr_controller					c_help;
	ptr_controller					c_file_browser;
	//[new-controller-property-mark]
};

}
