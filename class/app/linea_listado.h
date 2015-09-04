#ifndef LINEA_LISTADO_H
#define LINEA_LISTADO_H

#include <string>

class Linea_listado
{
	public:

				Linea_listado(size_t, const std::string&);
	size_t			acc_indice_animacion() const {return indice_animacion;}
	const std::string	acc_texto() const {return texto;}

	private:

	size_t			indice_animacion;
	std::string		texto;
};

#endif
