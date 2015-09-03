#include "controlador_intro.h"
#include "../app/recursos.h"

Controlador_intro::Controlador_intro(Director_estados &DI, DLibV::Pantalla& pantalla)
	:Controlador_base(DI),
	rep_txt(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), "Hola!")
{
	rep_txt.establecer_posicion(16, 400);
}

Controlador_intro::~Controlador_intro()
{
}

void Controlador_intro::preloop(Input_base& input, float delta)
{

}

void Controlador_intro::postloop(Input_base& input, float delta)
{

}

void Controlador_intro::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::I_ESPACIO))
		{
			solicitar_cambio_estado(Director_estados::t_estados::EJEMPLO);
		}
	}
}

void Controlador_intro::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
	rep_txt.volcar(pantalla);
}
