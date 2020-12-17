#pragma once

#include <iostream>

#define must_throw(_f, _type, _message){\
	try{\
		_f;\
		std::cerr<<"error: "<<_type<<": should have thrown with \""<<_message<<"\""<<std::endl;\
		abort();\
	}\
	catch(std::exception& e){\
		std::string err{e.what()};\
		if(std::string::npos==err.find(_message) ){\
			std::cerr<<"error: "<<_type<<": exception message does not contain \""<<_message<<"\" was \""<<err<<"\""<<std::endl;\
			exit(0);\
		}\
	}\
	std::cout<<"ok: "<<_type<<std::endl;\
}

#define must_not_throw(_f, _type){\
	try{\
		_f;\
		std::cout<<"ok: "<<_type<<std::endl;\
	}\
	catch(std::exception& e){\
		std::cerr<<"error: "<<_type<<": must not throw \""<<e.what()<<"\""<<std::endl;\
		exit(1);\
	}\
}

#define assert_same(_a, _b, _type){\
	if(_a != _b) {\
		std::cerr<<"error: "<<_type<<": failed to assert that lhs: '"<<_a<<"' is the same as rhs:'"<<_b<<"'"<<std::endl;\
		exit(1);\
	}\
	std::cout<<"ok: "<<_type<<std::endl;\
}
