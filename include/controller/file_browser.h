#pragma once

//local
#include "states.h"
#include "env/env.h"

//framework
#include <dfw/controller_interface.h>

//logger
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <ldtools/ttf_manager.h>
#include <tools/pager.h>

//std
#include <tools/file_utils.h>

namespace controller {

class file_browser:
	public dfw::controller_interface {

	public:

	                            file_browser(lm::logger&, ldtools::ttf_manager&, const env::env_interface&, int);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/) {}
	virtual bool                can_leave_state() const {return true;}

	void                        intra_set_allow_create(bool _v) {allow_create=_v;}
	bool                        intra_get_success() const {return intra_success;}
	const std::string&          intra_get_result() const {return intra_result;};

	private:

	enum class working_modes{navigate, create};

	struct entry {
		enum class entry_type {file, dir, create};
		std::string             path_name;
		entry_type              type;
		bool                    is_dir() const {return entry_type::dir==type;}
		bool                    is_new() const {return entry_type::create==type;}
		bool                    is_file() const {return entry_type::file==type;}
		bool                    operator<(const entry& _other) const {

			if(_other.is_new() && !is_new()) {
				return false;
			}

			if(is_new() && !_other.is_new()) {
				return true;
			}

			if(_other.is_dir() && !is_dir()) {
				return false;
			}

			if(is_dir() && !_other.is_dir()) {
				return true;
			}

			return path_name < _other.path_name;
		}
	};

	//!Does the navigation input.
	void                        input_navigation(dfw::input&);
	//!Does the input for new names.
	void                        input_create(dfw::input&);
	//!Fills up "contents" with the contents of the current directory.
	void						extract_entries();
	//!Refreshes the ttf_representation.
	void						refresh_list_view();
	//!Positions the current selector representation.
	void                        position_selector();
	//!Void composes the title
	void                        compose_title();
	//!Void sets the exchange data...
	void                        solve(bool, const std::string&);


	//references...
	lm::logger&                 log;
	ldtools::ttf_manager&       ttf_manager;

	//properties
	working_modes				mode;
	tools::filesystem::path     current_directory;
	std::string                 title;
	std::vector<entry>          contents;
	int                         first_selection_y{0},
	                            y_selection_factor{0};
	bool                        allow_create{true}; //!< True if it allows the [new] entry.

	tools::pager                pager;

	//view properties.
	ldtools::view_composer		layout;

	bool                        intra_success{false};
	std::string                 intra_result;
};

}
