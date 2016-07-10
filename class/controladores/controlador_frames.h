#ifndef CONTROLADOR_FRAMES
#define CONTROLADOR_FRAMES

#include <sstream>
#include <templates/listado_vertical.h>

#include "controlador_base.h"
#include "../app/tabla_animaciones.h"

class Controlador_frames:public Controlador_base
{
	///////////////
	//Interface pública.
	public:
	
							Controlador_frames(Director_estados &DI, DLibV::Pantalla&, const DLibV::Fuente_TTF&, Tabla_sprites&);

	virtual void 					preloop(Input_base& input, float delta);
	virtual void 					postloop(Input_base& input, float delta);
	virtual void 					loop(Input_base& input, float delta);
	virtual void 					dibujar(DLibV::Pantalla& pantalla);
	void						asignar_animacion(Animacion&);
	void						asignar_frame(size_t);

	//////////////
	//Propiedades
	private:

	struct item_listado
	{
		size_t		indice;
		std::string	texto;
	};

	DLibV::Pantalla&				pantalla;
	const DLibV::Fuente_TTF&			fuente;
	Animacion *					animacion;
	Tabla_sprites&					tabla_sprites;
	Herramientas_proyecto::Listado_vertical<item_listado>			listado;

	DLibV::Representacion_agrupada 			rep_listado;
	DLibV::Representacion_primitiva_caja	 	rep_seleccion_actual;
	DLibV::Representacion_bitmap	 		rep_animacion;

	float						tiempo_pulsado;
	size_t						indice_frame_nuevo;

	void						componer_lista();
	void						componer_vista_lista();
	void						calcular_posicion_seleccion_actual();
	void						calcular_representacion_frame_actual();
	void						cambiar_duracion_frame(int, float);

	static const size_t				y_inicio_lista=16;
	static const size_t				altura_linea=13;


};

#endif
