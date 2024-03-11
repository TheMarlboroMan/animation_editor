#include "app/animation_loader.h"

#include <tools/file_utils.h>
#include <tools/string_reader.h>
#include <sstream>

using namespace animation_editor;

animations animation_loader::from_string(
	const std::string& _input,
	bool _strict
) {

	animations result;
	animation anim;
	auto insert_animation=[&anim, &result, _strict]() {

		if(!anim.frames.size() && _strict) {

			throw std::runtime_error(std::string{"invalid animation contents, empty animation with no frames, current animation: "}+anim.name);
		}

		result.push_back(anim);

		//Clear.
		anim.name="";
		anim.frames.clear();
		anim.id=0;
	};

	tools::string_reader reader(_input, tools::string_reader::ltrim | tools::string_reader::rtrim);
	enum class modes{none, name, id, data};
	modes mode{modes::none};

	while(true) {

		std::string line=reader.read_line();

		std::stringstream ss{line};
		if(reader.is_eof()) {
			break;
		}

		switch(ss.peek()) {

			case '*':
				if(! (mode==modes::none || mode==modes::data)) {

					throw std::runtime_error("invalid animation contents, * found not following animation data or at the beginning");
				}

				ss.get(); //discard mark.

				//First animation, ok, give a name and go.
				if(mode==modes::none) {
					std::getline(ss, anim.name);
					mode=modes::name;
					continue;
				}

				//Any other, try to insert.
				insert_animation();
				std::getline(ss, anim.name);
				mode=modes::name;

			break;
			case '!':
				if(mode!=modes::name) {
					throw std::runtime_error(std::string{"invalid animation contents, ! found not following animation name, current animation: "}+anim.name);
				}
				ss.get(); //discard mark.
				ss>>anim.id;

				if(ss.fail()) {
					throw std::runtime_error(std::string{"invalid animation contents, bad id, current animation: "}+anim.name);
				}

				mode=modes::data;
			break;
			default: {
				if(mode!=modes::data) {

					throw std::runtime_error(std::string{"invalid animation contents, found data not following animation id, current animation: "}+anim.name);
				}

				int duration_ms{0};
				int flipped{0};
				std::size_t index{0};

				ss>>duration_ms>>index;

				if(ss.fail()) {
					throw std::runtime_error(std::string{"invalid animation contents, bad data, current animation: "}+anim.name);
				}

				ss>>flipped;
				if(ss.fail()) {

					//nothing, this is the old format...
				}

				bool flipped_horizontal=flipped & 1;
				bool flipped_vertical=flipped & 2;

				int degrees=0;
				if(flipped & 4) {

					degrees+=90;
				}

				if(flipped & 8) {

					degrees+=180;
				}

				anim.frames.push_back({index, duration_ms, flipped_horizontal, flipped_vertical, degrees});
			}
			break;
		}
	}

	//Add the last one.
	insert_animation();

	return result;
}

animations animation_loader::from_file(
	const std::string& _filename,
	bool _strict
) {

	return from_string(tools::dump_file(_filename), _strict);
}
