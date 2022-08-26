#include "../../include/env/env.h"

using namespace env;

dir_env::dir_env(
	const std::string& _path,
	const std::string& _homedir
):
	exec_dir{_path},
	user_dir{_homedir+"/.animation_editor/"}
{}

const std::string dir_env::build_data_path(const std::string& _file) const {

	return exec_dir+std::string{"resources/"}+_file;
}

const std::string dir_env::build_user_path(const std::string& _file) const {

	return user_dir+_file;
}

appimage_env::appimage_env(
	const std::string& _path,
	const std::string& _homedir
):
	exec_dir{std::string{"../share"}+_path},
	user_dir{_homedir+"/.animation_editor/"}
{

}

const std::string appimage_env::build_data_path(const std::string& _file) const {


	return exec_dir+std::string{"data/"}+_file;
}

const std::string appimage_env::build_user_path(const std::string& _file) const {

	return user_dir+_file;
}
