
#pragma once

#include <ldv/texture.h>
#include <ldtools/sprite_table.h>
#include <memory>

namespace animation_editor {

//!Lifetime manager for the texture and tileset.
class visuals {

	public:

	ldv::texture&                   get_texture() const {return *texture;}
	const ldtools::sprite_table&    get_table() const {return table;}

	void                            load_texture(const std::string&);
	void                            load_table(const std::string&);

	private:

	std::unique_ptr<ldv::texture>   texture{nullptr};
	ldtools::sprite_table           table;
};
}
