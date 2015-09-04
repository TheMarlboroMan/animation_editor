#include "estructura_paginacion.h"

#include <iostream>

/**
* @param size_t alt : altura de la línea.
* @param size_t t : total de elementos
* @param size_t h : altura del espacio reservado para el listado.
*/

Estructura_paginacion::Estructura_paginacion(size_t alt, size_t t, size_t h)
	:altura_linea(alt), h(h), total_elementos(t), 
	registros_por_pagina(0), pagina_actual(0),
	total_paginas(0), indice_actual(0)
{
	//Prevenir una división por 0...
	if(altura_linea && h)
	{
		calcular_informacion_paginas();
	}
}

void Estructura_paginacion::establecer_altura(size_t ph)
{
	h=ph;
	calcular_informacion_paginas();
}

void Estructura_paginacion::establecer_total_paginas(size_t val)
{
	total_elementos=val;
	calcular_informacion_paginas();
}

void Estructura_paginacion::calcular_informacion_paginas()
{
	registros_por_pagina=floor(h / altura_linea);
	total_paginas=ceil(total_elementos / registros_por_pagina);
}

bool Estructura_paginacion::cambiar_pagina(int val)
{
	if(val > 0 && pagina_actual + 1 <= total_paginas)
	{
		++pagina_actual;
		indice_actual+=registros_por_pagina;
		return true;
	}
	else if(val < 0 && pagina_actual > 0)
	{
		--pagina_actual;
		indice_actual-=registros_por_pagina;
		return true;
	}
	else return false;
}

bool Estructura_paginacion::cambiar_item(int val)
{
	bool resultado=false;

	if(val > 0 && indice_actual + 1 < total_elementos)
	{
		++indice_actual;
		resultado=true;
	}
	else if(val < 0 && indice_actual > 0)
	{
		--indice_actual;
		resultado=true;
	}
	
	if(resultado)
	{
		//Detectar si vamos a cambiar de página.
		size_t indice=indice_actual % (registros_por_pagina+1);
		if(indice==registros_por_pagina)
		{
			cambiar_pagina(val);
		}
	}

	return resultado;
}
