#include "app/animation_saver.h"
#include "test_macros.h"

#include <iostream>

int main(int, char **) {

	using namespace animation_editor;
	animation_saver saver;
	animations subject{};

	//empty
	subject.clear();
	must_throw(saver.to_string(subject), "empty animations", "no animations found");

	//repeated id
	subject.clear();
	subject.push_back({1, "name", {}});
	subject[0].frames.push_back({1, 2});
	subject.push_back({1, "other name", {}});
	must_throw(saver.to_string(subject), "repeated ids", "repeated index");

	//with no frames
	subject.clear();
	subject.push_back({1, "name", {}});
	must_throw(saver.to_string(subject), "no frames in animation", "no frames");

	//regular one
	subject.clear();
	subject.push_back({1, "name one", {}});
	subject[0].frames.push_back({1, 2});
	subject[0].frames.push_back({3, 4});
	subject.push_back({2, "name two", {}});
	subject[1].frames.push_back({5, 6});
	subject[1].frames.push_back({7, 8});

	std::string result;
	must_not_throw(result=saver.to_string(subject), "save good frame table");

	std::string expected{R"STR(*name one
!1
2	1
4	3
*name two
!2
6	5
8	7
)STR"};
	assert_same(result, expected, "result is as expected");

	std::cout<<"all good"<<std::endl;
	return 0;
}
