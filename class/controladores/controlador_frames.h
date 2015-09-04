#ifndef CONTROLADOR_FRAMES
#define CONTROLADOR_FRAMES

#include <sstream>

#include "controlador_base.h"
#include "../herramientas_proyecto/tabla_animaciones.h"

class Controlador_frames:public Controlador_base
{
	///////////////
	//Interface pública.
	public:
	
							Controlador_frames(Director_estados &DI, DLibV::Pantalla&);

	virtual void 					preloop(Input_base& input, float delta);
	virtual void 					postloop(Input_base& input, float delta);
	virtual void 					loop(Input_base& input, float delta);
	virtual void 					dibujar(DLibV::Pantalla& pantalla);

	//////////////
	//Propiedades
	private:

//	DLibV::Representacion_texto_auto_estatica 	rep_txt;
//	DLibV::Representacion_primitiva_caja_estatica 	rep_seleccion_frame_actual;
//	DLibV::Representacion_bitmap_estatica 		rep_animacion;
//	Animacion					animacion;

//	Tabla_animaciones&				tabla_animaciones;
//	std::vector<Linea_listado>			lineas_animaciones;

//	size_t						registros_por_pagina;
//	size_t						pagina_actual;
//	size_t						total_paginas;
//	size_t						animacion_actual;
//	float						tiempo;

//	void						componer_lista();
//	void						componer_vista_lista();
//	void						calcular_informacion_paginas(int h);
//	void						cambiar_pagina(int);
//	void						cambiar_animacion(int);
//	void						calcular_animacion_actual();
//	void						calcular_posicion_seleccion_animacion_actual();

	static const size_t				y_inicio_lista=16;
	static const size_t				altura_linea=13;


};

#endif
