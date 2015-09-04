#ifndef CONTROLADOR_ANIMACIONES
#define CONTROLADOR_ANIMACIONES

#include <sstream>

#include "controlador_base.h"
#include "../app/linea_listado.h"
#include "../herramientas_proyecto/tabla_animaciones.h"
#include "../herramientas_proyecto/estructura_paginacion.h"

class Controlador_animaciones:public Controlador_base
{
	///////////////
	//Interface pública.
	public:
	
							Controlador_animaciones(Director_estados &DI, DLibV::Pantalla&, Tabla_animaciones&);

	virtual void 					preloop(Input_base& input, float delta);
	virtual void 					postloop(Input_base& input, float delta);
	virtual void 					loop(Input_base& input, float delta);
	virtual void 					dibujar(DLibV::Pantalla& pantalla);

	void						refrescar();
	size_t						acc_indice_animacion_actual() const {return lineas_listado[estructura_paginacion.acc_indice_actual()].acc_indice();}

	//////////////
	//Propiedades
	private:

	DLibV::Representacion_texto_auto_estatica 	rep_txt;
	DLibV::Representacion_primitiva_caja_estatica 	rep_seleccion_actual;
	DLibV::Representacion_bitmap_estatica 		rep_animacion;
	Animacion					animacion;

	Tabla_animaciones&				tabla_animaciones;
	std::vector<Linea_listado>			lineas_listado;

	Estructura_paginacion				estructura_paginacion;

	float						tiempo;

	void						componer_lista();
	void						componer_vista_lista();
	void						calcular_animacion_actual();
	void						calcular_posicion_seleccion_actual();

	static const size_t				y_inicio_lista=16;
	static const size_t				altura_linea=13;


};

#endif
