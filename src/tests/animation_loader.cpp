#include "app/animation_loader.h"
#include "test_macros.h"

#include <iostream>

void run_tests(bool);
std::string msg(const char *, bool);

int main(int, char **) {

	run_tests(true);
	run_tests(false);
	std::cout<<"all done"<<std::endl;

	return 0;
}

void run_tests(
	bool _strict
) {
	using namespace animation_editor;
	animation_loader loader;


	std::string contents{R"STR(
not really a valid file
)STR"};
	must_throw(loader.from_string(contents, _strict), msg("invalid config file contents", _strict), "invalid animation contents");

	//id without name...
	contents=R"STR(
!33
)STR";
	must_throw(loader.from_string(contents, _strict), msg("id before name", _strict),  "! found not following animation name");

	//id without name...
	contents=R"STR(
*animation
!lol
)STR";
	must_throw(loader.from_string(contents, _strict), msg("bad id", _strict),  "bad id");

	//empty node
	contents=R"STR(
*animation
!1
*another
)STR";
	if(_strict) {
		must_throw(loader.from_string(contents, _strict), msg("empty animation", _strict),  "empty animation with no frames");
	}
	else {
		must_not_throw(loader.from_string(contents, _strict), msg("empty animation with no frames", _strict));
	}

	//bad frames
	contents=R"STR(
*animation
!1
lol lol
)STR";
	must_throw(loader.from_string(contents, _strict), msg("bad frames (both)", _strict),  "bad data");

	//bad frames
	contents=R"STR(
*animation
!1
1 lol
)STR";
	must_throw(loader.from_string(contents, _strict), msg("bad frames (just one)", _strict),  "bad data");

	//bad frames
	contents=R"STR(
*animation
!1
lol 2
)STR";
	must_throw(loader.from_string(contents, _strict), msg("bad frames (the other)", _strict),  "bad data");

	//good one
contents=R"STR(*animation 1
!1
1 2
3 4
*animation 2
!2
5 6
)STR";

	animations collection{};

	must_not_throw(collection=loader.from_string(contents), msg("load good data", _strict)  );
	assert_same(collection.size(), 2, msg("there are two animations", _strict));
	assert_same(collection[0].id, 1, msg("id of first animation", _strict));
	assert_same(collection[0].name, "animation 1", msg("name of first animation", _strict));
	assert_same(collection[0].frames.size(), 2, msg("frame count of first animation", _strict));
	assert_same(collection[0].frames[0].index, 2, msg("first animation, first frame index", _strict));
	assert_same(collection[0].frames[0].duration_ms, 1, msg("first animation, first frame duration", _strict));
	assert_same(collection[0].frames[1].index, 4, msg("first animation, second frame index", _strict));
	assert_same(collection[0].frames[1].duration_ms,3, msg("first animation, second frame duration", _strict));
	assert_same(collection[1].id, 2, msg("id of second animation", _strict));
	assert_same(collection[1].name, "animation 2", msg("name of second animation", _strict));
	assert_same(collection[1].frames.size(), 1,msg("frame count of second animation", _strict));
	assert_same(collection[1].frames[0].index, 6, msg("second animation, first frame index", _strict));
	assert_same(collection[1].frames[0].duration_ms, 5, msg("second animation, first frame duration", _strict));
}

std::string msg(
	const char * _msg,
	bool _strict
) {

	std::string result{_msg};
	result+=_strict ? " (strict)" : " (non-strict)";
	return result;
}
