#ifndef CONTROLADOR_ANIMACIONES
#define CONTROLADOR_ANIMACIONES

#include "controlador_base.h"

class Controlador_animaciones:public Controlador_base
{
	//////////////
	//Propiedades
	private:

	DLibV::Representacion_texto_auto_estatica rep_txt;

	///////////////
	//Métodos internos.

	///////////////
	//Interface pública.
	public:
	
	Controlador_animaciones(Director_estados &DI, DLibV::Pantalla&);
	~Controlador_animaciones();

	virtual void preloop(Input_base& input, float delta);
	virtual void postloop(Input_base& input, float delta);
	virtual void loop(Input_base& input, float delta);
	virtual void dibujar(DLibV::Pantalla& pantalla);
};

#endif
