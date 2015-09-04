#include "controlador_frames.h"
#include "../app/recursos.h"

Controlador_frames::Controlador_frames(Director_estados &DI, DLibV::Pantalla& pantalla)
	:Controlador_base(DI)
{

}

void Controlador_frames::preloop(Input_base& input, float delta)
{

}

void Controlador_frames::postloop(Input_base& input, float delta)
{

}

void Controlador_frames::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::I_ESCAPE))
		{
			solicitar_cambio_estado(Director_estados::t_estados::animaciones);
			return;
		}
	}
}

void Controlador_frames::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
}
