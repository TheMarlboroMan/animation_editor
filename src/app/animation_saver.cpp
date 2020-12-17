#include "app/animation_saver.h"

#include <fstream>
#include <sstream>
#include <set>
#include <stdexcept>

using namespace animation_editor;


std::string animation_saver::to_string(
	const animations& _animations
) {
	if(!_animations.size()) {

		throw std::runtime_error("no animations found");
	}

	std::set<std::size_t> ids;
	for(const auto& anim : _animations) {

		if(ids.count(anim.id)) {

			throw std::runtime_error(std::string{"repeated index "}+std::to_string(anim.id));
		}
		ids.insert(anim.id);

		if(!anim.frames.size()) {

			throw std::runtime_error(std::string{"no frames in animation "}+anim.name);
		}
	}

	std::stringstream ss;
	for(const auto& anim : _animations) {

		ss<<"*"<<anim.name<<std::endl<<"!"<<anim.id<<std::endl;
		for(const auto& fr : anim.frames) {

			ss<<fr.duration_ms<<"\t"<<fr.index<<std::endl;
		}
	}

	return ss.str();
}

bool animation_saver::to_file(
	const std::string& _filename,
	const animations& _animations
) {

	std::ofstream file{_filename};
	if(!file) {

		return false;
	}

	file<<to_string(_animations);
}
