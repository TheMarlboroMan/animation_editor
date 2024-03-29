#include "app/animation_saver.h"

#include <fstream>
#include <sstream>
#include <set>
#include <stdexcept>

using namespace animation_editor;


std::string animation_saver::to_string(
	const animations& _animations,
	bool _strict
) {
	if(!_animations.size() && _strict) {

		throw std::runtime_error("no animations found");
	}

	std::set<std::size_t> ids;
	for(const auto& anim : _animations) {

		if(ids.count(anim.id)) {

			throw std::runtime_error(std::string{"repeated index "}+std::to_string(anim.id));
		}
		ids.insert(anim.id);

		if(!anim.frames.size() && _strict) {

			throw std::runtime_error(std::string{"no frames in animation "}+anim.name);
		}
	}

	std::stringstream ss;
	for(const auto& anim : _animations) {

		ss<<"*"<<anim.name<<std::endl<<"!"<<anim.id<<std::endl;
		for(const auto& fr : anim.frames) {

			int flipped=0;
			if(fr.flipped_horizontal) flipped+=1;
			if(fr.flipped_vertical) flipped+=2;

			switch(fr.rotation_degrees) {

				case 90: flipped+=4; break;
				case 180: flipped+=8; break;
				case 270: flipped+=12; break;
			}

			ss<<fr.duration_ms<<"\t"<<fr.index<<"\t"<<flipped<<std::endl;
		}
	}

	return ss.str();
}

void animation_saver::to_file(
	const std::string& _filename,
	const animations& _animations,
	bool _strict
) {

	std::ofstream file{_filename};
	if(!file) {

		throw std::runtime_error("could not write to file");
	}

	file<<to_string(_animations, _strict);
}
