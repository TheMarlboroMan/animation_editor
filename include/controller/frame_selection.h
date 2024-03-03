#pragma once

//local
#include "states.h"
#include "app/visuals.h"
#include "app/frame.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>
#include <tools/grid_list.h>
#include <ldtools/animation_table.h>

namespace controller {

class frame_selection:
	public dfw::controller_interface {

	public:

	                            frame_selection(
	                                lm::logger&,
	                                ldtools::ttf_manager&,
	                                const animation_editor::visuals&,
	                                ldv::rect,
	                                unsigned int,
	                                unsigned int
	                            );
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/) {}
	virtual bool                can_leave_state() const {return true;}

	bool                        intra_get_success() const {return intra_success;}
	std::size_t                 intra_get_frame_id() const {return frame_list.get().first;}

	private:

	//references...
	lm::logger&                         log;
	ldtools::ttf_manager&               ttf_manager;
	const animation_editor::visuals&    visuals;

	//properties

	ldv::rect                                   display_rect;
	tools::grid_list<ldtools::sprite_table::container::value_type> frame_list;
	bool                                        intra_success{false};
	int                                         last_page{0};
	int                                         last_index{0};
};

}
