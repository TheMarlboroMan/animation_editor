#ifndef CONTROLADOR_SELECTOR
#define CONTROLADOR_SELECTOR

#include <sstream>

#include "controlador_base.h"
#include "../herramientas_proyecto/tabla_sprites.h"
#include "../herramientas_proyecto/listado_rejilla.h"

class Controlador_selector:public Controlador_base
{
	///////////////
	//Interface pública.
	public:
	
							Controlador_selector(Director_estados &DI, DLibV::Pantalla&, Tabla_sprites&);

	virtual void 					preloop(Input_base& input, float delta);
	virtual void 					postloop(Input_base& input, float delta);
	virtual void 					loop(Input_base& input, float delta);
	virtual void 					dibujar(DLibV::Pantalla& pantalla);
	size_t						acc_indice_frame() const {return indice_frame;}

	//////////////
	//Propiedades
	private:

	struct item_listado
	{
		size_t		indice;
		Frame_sprites	frame;
	};

	DLibV::Pantalla&				pantalla;
	Tabla_sprites&					tabla_sprites;
	Listado_rejilla<item_listado>			listado;

	DLibV::Representacion_agrupada_estatica 	rep_listado;
	DLibV::Representacion_primitiva_caja_estatica 	rep_seleccion_actual;

	size_t						indice_frame;

	void						componer_lista();
	void						componer_vista_lista();
	void						calcular_posicion_seleccion_actual();

	static const size_t				inicio_lista=16;
	static const size_t				dim_item_lista=40;
};

#endif
