#include "dfwimpl/config.h"
#include "dfwimpl/state_driver.h"
#include "env/env.h"

#include <lm/file_logger.h>
#include <lm/log.h>

#include <dfw/kernel.h>

#include <tools/arg_manager.h>
#include <tools/file_utils.h>

#include <ldt/sdl_tools.h>
#include <ldt/log.h>
#include <ldt/lib.h>
#include <ldtools/lib.h>
#include <dfw/lib.h>

#include <memory>
#include <stdexcept>

#include <unistd.h>
#include <cstdlib>

std::unique_ptr<env::env_interface> make_env();

int main(int argc, char ** argv)
{
	auto env=make_env();

	//Init libdansdl2 log.
#ifdef IS_DEBUG_BUILD
	ldt::log_lsdl::set_type(ldt::log_lsdl::types::out);
#else
	ldt::log_lsdl::set_type(ldt::log_lsdl::types::null);
#endif

	//Argument controller.
	tools::arg_manager carg(argc, argv);

	if(carg.exists("-h")) {

		std::cout<<"animation_editor "
			<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"-"<<BUILD_VERSION
			<<" built on "<<__DATE__<<" "<<__TIME__
			<<std::endl<<std::endl
			<<tools::dump_file(env->build_data_path("assets/texts/help.txt"))<<std::endl;
		return 0;
	}

	if(carg.exists("-v")) {

		std::cout<<"animation_editor "
			<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"-"<<BUILD_VERSION
			<<" built on "<<__DATE__<<" "<<__TIME__<<std::endl
			<<"libdansdl2 version: "<<ldt::get_lib_version()<<std::endl
			<<"ldtools version: "<<ldtools::get_lib_version()<<std::endl
			<<"dfw version: "<<dfw::get_lib_version()<<std::endl;
		return 0;
	}

	//Init application log.
	const std::string log_path{env->build_user_path("app.log")};
	lm::file_logger log_app(log_path.c_str());
	lm::log(log_app).info()<<"starting main process..."<<std::endl;

	//Init...
	try {
		lm::log(log_app).info()<<"init sdl2..."<<std::endl;
		if(!ldt::sdl_init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK)) {
			throw std::runtime_error("unable to init sdl2");
		}

		lm::log(log_app).info()<<"creating kernel..."<<std::endl;
		dfw::kernel kernel(log_app, carg);

		lm::log(log_app).info()<<"init app config..."<<std::endl;
		dfwimpl::config config{*(env)};

		lm::log(log_app).info()<<"create state driver..."<<std::endl;
		dfwimpl::state_driver sd(kernel, config, *(env));

		lm::log(log_app).info()<<"init state driver..."<<std::endl;
		sd.init(kernel);
		sd.start(kernel);

		lm::log(log_app).info()<<"finish main proccess"<<std::endl;
	}
	catch(std::exception& e) {
		std::cout<<"Interrupting due to exception: "<<e.what()<<std::endl;
		lm::log(log_app).error()<<"an error happened "<<e.what()<<std::endl;
		lm::log(log_app).info()<<"stopping sdl2..."<<std::endl;
		ldt::sdl_shutdown();

		return 1;
	}

	lm::log(log_app).info()<<"stopping sdl2..."<<std::endl;
	ldt::sdl_shutdown();
	return 0;
}

std::unique_ptr<env::env_interface> make_env() {

	std::string executable_path, executable_dir;
	std::array<char, 1024> buff;

	int bytes=readlink("/proc/self/exe", buff.data(), 1024);
	if(-1==bytes) {

		std::cerr<<"could not locate proc/self/exe, error "<<errno<<std::endl;
		throw std::runtime_error("could not locate proc/self/exe");
	}

	executable_path=std::string{std::begin(buff), std::begin(buff)+bytes};
	auto last_slash=executable_path.find_last_of("/");
	executable_dir=executable_path.substr(0, last_slash)+"/";

#ifdef AS_REGULAR

	auto result=std::unique_ptr<env::env_interface>(
		new env::dir_env(executable_dir, getenv("HOME"))
	);

#else
	#ifdef AS_APPIMAGE

	auto result=std::unique_ptr<env::env_interface>(
		new env::appimage_env(executable_dir, getenv("HOME"))
	);

	#else
		#error AS_REGULAR or AS_APPIMAGE must be defined
	#endif
#endif
	if(!tools::filesystem::exists(result->build_user_path(""))) {

		std::cout<<"will create the .animation_editor under user home"<<std::endl;
		tools::filesystem::create_directory(result->build_user_path(""));
	}

	if(!tools::filesystem::exists(result->build_user_path("config.json"))) {

		std::cout<<"will copy config.json file"<<std::endl;
		tools::filesystem::copy(
			result->build_data_path("config.json"),
			result->build_user_path("config.json")
		);
	}

	return result;
}
