#include "controlador_animaciones.h"
#include "../app/recursos.h"

Controlador_animaciones::Controlador_animaciones(Director_estados &DI, DLibV::Pantalla& pantalla)
	:Controlador_base(DI),
	rep_txt(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), "Hola!")
{
	rep_txt.establecer_posicion(16, 400);
}

Controlador_animaciones::~Controlador_animaciones()
{
}

void Controlador_animaciones::preloop(Input_base& input, float delta)
{

}

void Controlador_animaciones::postloop(Input_base& input, float delta)
{

}

void Controlador_animaciones::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		abandonar_aplicacion();
	}
	else
	{

	}
}

void Controlador_animaciones::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
	rep_txt.volcar(pantalla);
}
