#include "app/animation_loader.h"
#include "test_macros.h"

#include <iostream>

int main(int, char **) {

	using namespace animation_editor;
	animation_loader loader;

	std::string contents{R"STR(
not really a valid file
)STR"};
	must_throw(loader.from_string(contents), "invalid config file contents", "invalid animation contents");

	//id without name...
	contents=R"STR(
!33
)STR";
	must_throw(loader.from_string(contents), "id before name", "! found not following animation name");

	//id without name...
	contents=R"STR(
*animation
!lol
)STR";
	must_throw(loader.from_string(contents), "bad id", "bad id");

	//empty node
	contents=R"STR(
*animation
!1
*another
)STR";
	must_throw(loader.from_string(contents), "empty animation", "empty animation with no frames");

	//bad frames
	contents=R"STR(
*animation
!1
lol lol
)STR";
	must_throw(loader.from_string(contents), "bad frames (both)", "bad data");

	//bad frames
	contents=R"STR(
*animation
!1
1 lol
)STR";
	must_throw(loader.from_string(contents), "bad frames (just one)", "bad data");

	//bad frames
	contents=R"STR(
*animation
!1
lol 2
)STR";
	must_throw(loader.from_string(contents), "bad frames (the other)", "bad data");

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

	must_not_throw(collection=loader.from_string(contents), "load good data");
	assert_same(collection.size(), 2, "there are two animations");
	assert_same(collection[0].id, 1, "id of first animation");
	assert_same(collection[0].name, "animation 1", "name of first animation");
	assert_same(collection[0].frames.size(), 2, "frame count of first animation");
	assert_same(collection[0].frames[0].index, 2, "first animation, first frame index");
	assert_same(collection[0].frames[0].duration_ms, 1, "first animation, first frame duration");
	assert_same(collection[0].frames[1].index, 4, "first animation, second frame index");
	assert_same(collection[0].frames[1].duration_ms,3, "first animation, second frame duration");
	assert_same(collection[1].id, 2, "id of second animation");
	assert_same(collection[1].name, "animation 2", "name of second animation");
	assert_same(collection[1].frames.size(), 1, "frame count of second animation");
	assert_same(collection[1].frames[0].index, 6, "second animation, first frame index");
	assert_same(collection[1].frames[0].duration_ms, 5, "second animation, first frame duration");

	std::cout<<"all done"<<std::endl;

	return 0;
}
