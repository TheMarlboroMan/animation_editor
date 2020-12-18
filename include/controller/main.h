#pragma once

//local
#include "states.h"
#include "tools/ticker.h"
#include "tools/message_manager.h"
#include "app/animation.h"
#include "app/visuals.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>
#include <tools/vertical_list.h>

namespace controller {

class main:
	public dfw::controller_interface {

	public:

	                            main(lm::logger&,
	                                const ldtools::ttf_manager&,
	                                tools::message_manager&,
	                                const tools::ticker&,
	                                animation_editor::animations&,
	                                const animation_editor::visuals&,
	                                ldv::rect,
	                                int,
	                                int);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	bool                        intra_get_file_browser_allow_new() const {return intra_file_browser_allow_new;}
	void                        intra_set_update_on_awake(bool _val) {intra_update_on_awake=_val;}

	private:

	void                        receive_message(tools::message_manager::notify_event_type);
	void                        update_hud();
	void                        update_list();
	void                        move_animation_up();
	void                        move_animation_down();
	void                        erase_animation();
	void                        insert_animation();

	//references...
	lm::logger&                         log;
	const ldtools::ttf_manager&         ttf_manager;
	tools::message_manager&             message_manager;
	const tools::ticker&                ticker;
	animation_editor::animations&       animations;
	const animation_editor::visuals&    visuals;

	//properties
	ldv::rect                           display_rect;
	ldtools::view_composer              layout;
	tools::vertical_list<animation_editor::animation>   animation_list;

	bool                                intra_file_browser_allow_new{false},
	                                    intra_update_on_awake{true};

	const int                           margin_top_list;
	const int                           h_list_item;

};

}
