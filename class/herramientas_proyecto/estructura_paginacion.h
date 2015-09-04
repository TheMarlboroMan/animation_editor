#ifndef ESTRUCTURA_PAGINACION_H
#define ESTRUCTURA_PAGINACION_H

/**
* Estructura de paginación que incluye un índice para seleccionar algo.
*/

#include <cstddef>
#include <cmath>

class Estructura_paginacion
{
	public:
							Estructura_paginacion(size_t alt, size_t total, size_t h);
	void						calcular_informacion_paginas();
	bool						cambiar_pagina(int);
	bool						cambiar_item(int);

	void						reiniciar_indice() {indice_actual=0;}
	size_t						acc_pagina_actual() const {return pagina_actual;}
	size_t						acc_indice_actual() const {return indice_actual;}
	size_t						acc_total_paginas() const {return total_paginas;}
	size_t						acc_registros_por_pagina() const {return registros_por_pagina;}

	void						establecer_altura(size_t);
	void						establecer_total_paginas(size_t);

	private:

	size_t						altura_linea;
	size_t						h;
	size_t						total_elementos;
	size_t						registros_por_pagina;
	size_t						pagina_actual;
	size_t						total_paginas;
	size_t						indice_actual;
};

#endif
