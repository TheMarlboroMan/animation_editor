#include "app/animation_saver.h"
#include "test_macros.h"

#include <iostream>

void run_tests(bool);
std::string msg(const char *, bool);

int main(int, char **) {

	run_tests(true);
	run_tests(false);

	std::cout<<"all good"<<std::endl;
	return 0;
}

void run_tests(
	bool _strict
) {
	using namespace animation_editor;
	animation_saver saver;
	animations subject{};

	//empty
	subject.clear();
	if(_strict) {
		must_throw(saver.to_string(subject, _strict), msg("empty animations", _strict), "no animations found");
	}
	else {
		must_not_throw(saver.to_string(subject, _strict), msg("empty animations", _strict));
	}

	//repeated id
	subject.clear();
	subject.push_back({1, "name", {}});
	subject[0].frames.push_back({1, 2});
	subject.push_back({1, "other name", {}});
	must_throw(saver.to_string(subject, _strict), msg("repeated ids", _strict), "repeated index");

	//with no frames
	subject.clear();
	subject.push_back({1, "name", {}});
	if(_strict) {
		must_throw(saver.to_string(subject, _strict), msg("no frames in animation", _strict), "no frames");
	}
	else {
		must_not_throw(saver.to_string(subject, _strict), msg("no frames in animation", _strict));
	}

	//regular one
	subject.clear();
	subject.push_back({1, "name one", {}});
	subject[0].frames.push_back({1, 2});
	subject[0].frames.push_back({3, 4});
	subject.push_back({2, "name two", {}});
	subject[1].frames.push_back({5, 6});
	subject[1].frames.push_back({7, 8});

	std::string result;
	must_not_throw(result=saver.to_string(subject, _strict), msg("save good frame table", _strict));

	std::string expected{R"STR(*name one
!1
2	1
4	3
*name two
!2
6	5
8	7
)STR"};
	assert_same(result, expected, msg("result is as expected", _strict));
}

std::string msg(
	const char * _msg,
	bool _strict
) {

	std::string result{_msg};
	result+=_strict ? " (strict)" : " (non-strict)";
	return result;
}
