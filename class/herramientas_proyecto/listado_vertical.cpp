#include "listado_vertical.h"

Listado_vertical::Listado_vertical(size_t altura_disponible, size_t altura_linea)
	:altura_disponible(altura_disponible), altura_linea(altura_linea)
{
	estructura_paginacion.establecer_registros_por_pagina(floor(altura_disponible_altura_linea));
}

void Listado_vertical::insertar(const T& v)
{
	lineas.push_back(Linea_listado(lineas.size(), v));
	estructura_paginacion.establecer_total_elementos(estructura_paginacion.acc_total_elementos()+1);
}

void Listado_vertical::clear()
{
	lineas.clear();
	estructura_paginacion.establecer_total_elementos(0);
}

std::vector<Item> Listado_vertical::obtener_pagina() const
{
	std::vector<Item> res;

	size_t rpp=estructura_paginacion().acc_registros_por_pagina();
	size_t pa=estructura_paginacion().acc_pagina_actual();

	auto	ini=std::begin(lineas)+(pa * rpp),
		fin=ini+rpp;

	int y=0;

	while(ini < fin && ini < std::end(lineas))
	{
		res.push_back(y, *ini);
		++ini;
		y+=altura_linea;
	}

	return res;
}
