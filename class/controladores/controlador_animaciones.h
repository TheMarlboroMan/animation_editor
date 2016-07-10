#ifndef CONTROLADOR_ANIMACIONES
#define CONTROLADOR_ANIMACIONES

#include <sstream>
#include <fstream>
#include <templates/listado_vertical.h>

#include "controlador_base.h"
#include "../app/tabla_animaciones.h"

class Controlador_animaciones:public Controlador_base
{
	///////////////
	//Interface pública.
	public:

	struct item_listado
	{
		size_t		indice;
		std::string	texto;
	};
	
							Controlador_animaciones(Director_estados &DI, DLibV::Pantalla&, Tabla_animaciones&, const DLibV::Fuente_TTF&, const std::string nombre_fichero);

	virtual void 					preloop(Input_base& input, float delta);
	virtual void 					postloop(Input_base& input, float delta);
	virtual void 					loop(Input_base& input, float delta);
	virtual void 					dibujar(DLibV::Pantalla& pantalla);

	void						refrescar();
	size_t						acc_indice_animacion_actual() const {return listado.item_actual().indice;}

	//////////////
	//Propiedades
	private:

	DLibV::Pantalla&				pantalla;
	const DLibV::Fuente_TTF&			fuente;
	DLibV::Representacion_agrupada 			rep_listado;
	DLibV::Representacion_primitiva_caja 		rep_seleccion_actual;
	DLibV::Representacion_bitmap	 		rep_animacion;
	Animacion					animacion;
	std::string					nombre_fichero;

	Tabla_animaciones&				tabla_animaciones;
	Herramientas_proyecto::Listado_vertical<item_listado>	listado;

	float						tiempo;

	void						componer_lista();
	void						componer_vista_lista();
	void						calcular_animacion_actual();
	void						calcular_posicion_seleccion_actual();
	void						guardar();

	static const size_t				y_inicio_lista=16;
	static const size_t				altura_linea=13;


};

#endif
