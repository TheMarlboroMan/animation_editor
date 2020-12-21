#pragma once

//local
#include "states.h"
#include "app/visuals.h"
#include "app/animation.h"
#include "tools/ticker.h"
#include "tools/message_manager.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>
#include <tools/vertical_list.h>

namespace controller {

class animation:
	public dfw::controller_interface {

	public:

	                            animation(
	                                lm::logger&,
	                                ldtools::ttf_manager&,
	                                tools::message_manager&,
	                                const tools::ticker&,
	                                const animation_editor::visuals&,
	                                ldv::rect,
	                                unsigned int,
	                                unsigned int
	                            );
		virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
		virtual void                draw(ldv::screen&, int);
		virtual void                awake(dfw::input& /*input*/);
		virtual void                slumber(dfw::input& /*input*/);
		virtual bool                can_leave_state() const {return true;}

		void                        intra_set_animation(animation_editor::animation& _anim) {current_animation=&_anim;}
		void                        intra_set_frame_index(std::size_t);

		private:

		void                        receive_message(tools::message_manager::notify_event_type);
		void                        update_hud();
		void                        update_list();
		void                        move_frame_up();
		void                        move_frame_down();
		void                        erase_frame();
		void                        insert_frame();
		void                        enter_animation_frames();
		void                        make_frame_longer(bool);
		void                        make_frame_shorter(bool);

		//references...
		lm::logger&                         log;
		ldtools::ttf_manager&               ttf_manager;
		tools::message_manager&             message_manager;
		const tools::ticker&                ticker;
		const animation_editor::visuals&    visuals;

		//properties
		animation_editor::animation *       current_animation{nullptr};
		int                                 duration{0};
		ldv::rect                           display_rect;
		ldtools::view_composer              layout;
		tools::vertical_list<animation_editor::frame> frame_list;
		bool                                preserve_animation_on_exit{false},
		                                    preserve_index_on_enter{false};

		const unsigned int                  margin_top_list;
		const unsigned int                  h_list_item;
	};
}
