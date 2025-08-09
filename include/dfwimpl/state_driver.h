#pragma once

#include "config.h"
#include "app/animation.h"
#include "app/visuals.h"
#include "env/env.h"
#include "tools/message_manager.h"
#include "tools/ticker.h"

#include <dfw/state_driver_interface.h>
#include <ldtools/ttf_manager.h>

#include <memory>

//Controllers.
#include "../../include/controller/main.h"
#include "../../include/controller/help.h"
#include "../../include/controller/file_browser.h"
#include "../../include/controller/rename_animation.h"
#include "../../include/controller/change_animation_id.h"
#include "../../include/controller/animation.h"
#include "../../include/controller/frame_selection.h"
//[new-controller-header-mark]

//Specific app_config
//#include "../app/placeholder.h"

namespace dfwimpl {

class state_driver:
	public dfw::state_driver_interface {

	public:
	                                state_driver(
		dfw::kernel& kernel, 
		dfwimpl::config& config,
		const env::env_interface&
	);

	virtual void                    common_pre_loop_input(dfw::input& input, ldtools::tdelta);
	virtual void                    common_pre_loop_step(ldtools::tdelta);
	virtual void                    common_loop_input(dfw::input& input, ldtools::tdelta);
	virtual void                    common_loop_step(ldtools::tdelta);
	virtual void                    prepare_state(int, int);
	virtual ldtools::tdelta         get_max_timestep() const {return 0.03;}
	virtual void                    init(dfw::kernel&);

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
	const env::env_interface&      env;

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
	ptr_controller					c_rename_animation;
	ptr_controller					c_change_animation_id;
	ptr_controller					c_animation;
	ptr_controller					c_frame_selection;
	//[new-controller-property-mark]
};

}
