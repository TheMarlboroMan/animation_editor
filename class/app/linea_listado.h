#ifndef LINEA_LISTADO_H
#define LINEA_LISTADO_H

#include <string>

//TODO: Quizás podamos crear una nueva clase uniendo esto con el control de
//paginación y haciendo que sea un "passthrough" para las cosas de paginación
//y tal... Algún tipo de "listado_paginado". Luego podríamos hacer un 
//"listado_rejilla" también.

template <typename T>
class Linea_listado
{
	public:

				Linea_listado(size_t indice, const T& val)
				:indice(indice), valor(val) {}
	
	size_t			acc_indice() const {return indice;}
	const T			acc_valor() const {return valor;}

	private:

	size_t			indice;
	T			valor;
};

#endif
