#ifndef LISTADO_VERTICAL_H
#define LISTADO_VERTICAL_H

#include "linea_listado.h"
#include "estructura_paginacion.h"

template<typename T>
class Listado_vertical
{
	public:

	struct Item
	{
		int y;
		const Linea_listado<T>& item;
	};

						//Propios...
						Listado_vertical(size_t altura_disponible, size_t altura_linea);
	void					size() const {return lineas.size();}

	void 					insertar(const T&);
	std::vector<Item>			obtener_pagina() const;
	void					clear();


	//Passthroughs paginación...
	bool					cambiar_pagina(int v) 	{estructura_paginacion.cambiar_pagina(v);}
	bool					cambiar_item(int v)	{estructura_paginacion.cambiar_item(v);}
	void					reiniciar_indice() 	{estructura_paginacion.reiniciar_indice();}
	size_t					acc_pagina_actual() const {return estructura_paginacion.acc_pagina_actual();}
	size_t					acc_indice_actual() const {return estructura_paginacion.acc_indice_actual();}
	size_t					acc_total_paginas() const {return estructura_paginacion.acc_total_paginas();}
	size_t					acc_registros_por_pagina() const {return estructura_paginacion.acc_registros_por_pagina();}

	

	private:

	Estructura_paginacion			estructura_paginacion;
	std::vector<Linea_listado<T>>		lineas;

	size_t					altura_disponible;
	size_t					altura_linea;
};

#endif
