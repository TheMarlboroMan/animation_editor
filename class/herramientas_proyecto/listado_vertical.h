#ifndef LISTADO_VERTICAL_H
#define LISTADO_VERTICAL_H

#include <iostream>
#include <vector>
#include "estructura_paginacion.h"

template<typename T>
class Listado_vertical
{
	public:


	//Estructura que se sirve como parte de un listado.
	struct Item
	{
		int y;
		const T& item;
	};

						//Propios...
						Listado_vertical(size_t altura_disponible, size_t altura_linea)
		:altura_disponible(altura_disponible), altura_linea(altura_linea)
	{
		estructura_paginacion.establecer_registros_por_pagina(floor(altura_disponible / altura_linea));
	}

	void					size() const 
	{
		return lineas.size();
	}

	const T&			operator[](size_t v) const 
	{
		return lineas[v];
	}

	const T&			item_actual() const 
	{
		return lineas[estructura_paginacion.acc_indice_actual()];
	}

	const Item			linea_actual() const 
	{
		return Item{(int)altura_linea * (int)estructura_paginacion.acc_indice_actual(),  item_actual()};
	}

	void 					insertar(const T& v)
	{
		lineas.push_back(v);
		estructura_paginacion.establecer_total_elementos(estructura_paginacion.acc_total_elementos()+1);
	}

	std::vector<Item>			obtener_pagina() const
	{
		std::vector<Item> res;

		size_t rpp=estructura_paginacion.acc_registros_por_pagina();
		size_t pa=estructura_paginacion.acc_pagina_actual();

		auto	ini=std::begin(lineas)+(pa * rpp),
			fin=ini+rpp;

		int y=0;

		while(ini < fin && ini < std::end(lineas))
		{
			res.push_back(Item{y, *ini});
			++ini;
			y+=(int)altura_linea;
		}

		return res;
	}

	void					clear()
	{
		lineas.clear();
		estructura_paginacion.establecer_total_elementos(0);
	}


	//Passthroughs paginación...
	bool					cambiar_pagina(int v) 	{return estructura_paginacion.cambiar_pagina(v);}
	bool					cambiar_item(int v)	{return estructura_paginacion.cambiar_item(v);}
	void					reiniciar_indice() 	{estructura_paginacion.reiniciar_indice();}
	size_t					acc_pagina_actual() const {return estructura_paginacion.acc_pagina_actual();}
	size_t					acc_indice_actual() const {return estructura_paginacion.acc_indice_actual();}
	size_t					acc_total_paginas() const {return estructura_paginacion.acc_total_paginas();}
	size_t					acc_registros_por_pagina() const {return estructura_paginacion.acc_registros_por_pagina();}

	protected:

	Estructura_paginacion			estructura_paginacion;
	std::vector<T>				lineas;

	size_t					altura_disponible;
	size_t					altura_linea;
};

#endif
