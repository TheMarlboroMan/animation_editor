#pragma once

#include "states.h"
#include "app/animation.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>

#include <string>

namespace controller {

class change_animation_id:
	public dfw::controller_interface {

	public:

	                            change_animation_id(lm::logger&, ldtools::ttf_manager&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}
	virtual void                request_draw(dfw::controller_view_manager&);

	void                        intra_set_animation(animation_editor::animation * _animation) {animation=_animation;}

	private:

	//references...
	lm::logger&                 log;
	ldtools::ttf_manager&       ttf_manager;

	//properties
	animation_editor::animation * animation{nullptr};
	std::string                 current_value;
};

}
