#ifndef CONTROLADOR_FRAMES
#define CONTROLADOR_FRAMES

#include <sstream>

#include "controlador_base.h"
#include "../herramientas_proyecto/tabla_animaciones.h"
#include "../herramientas_proyecto/estructura_paginacion.h"
#include "../herramientas_proyecto/linea_listado.h"

class Controlador_frames:public Controlador_base
{
	///////////////
	//Interface pública.
	public:
	
							Controlador_frames(Director_estados &DI, DLibV::Pantalla&, Tabla_sprites&);

	virtual void 					preloop(Input_base& input, float delta);
	virtual void 					postloop(Input_base& input, float delta);
	virtual void 					loop(Input_base& input, float delta);
	virtual void 					dibujar(DLibV::Pantalla& pantalla);
	void						asignar_animacion(Animacion&);

	//////////////
	//Propiedades
	private:

	Animacion *					animacion;
	Tabla_sprites&					tabla_sprites;

	DLibV::Representacion_texto_auto_estatica 	rep_txt;
	DLibV::Representacion_primitiva_caja_estatica 	rep_seleccion_actual;
	DLibV::Representacion_bitmap_estatica 		rep_animacion;

	std::vector<Linea_listado<std::string>>		lineas_listado;
	int						w;
	float						tiempo_pulsado;
	size_t						indice_frame_nuevo;

	Estructura_paginacion				estructura_paginacion;

	void						componer_lista();
	void						componer_vista_lista();
	void						calcular_posicion_seleccion_actual();
	void						calcular_representacion_frame_actual();
	void						cambiar_duracion_frame(int, float);

	static const size_t				y_inicio_lista=16;
	static const size_t				altura_linea=13;


};

#endif
