#include "controlador_ejemplo.h"

Controlador_ejemplo::Controlador_ejemplo(Director_estados &DI)
	:Controlador_base(DI), actual(0,0,0), deseado(0,0,0)
{
}

Controlador_ejemplo::~Controlador_ejemplo()
{
}

void Controlador_ejemplo::preloop(Input_base& input, float delta)
{
	if(actual!=deseado)
	{
		actual.aproximar_a(deseado);
	}
}

void Controlador_ejemplo::postloop(Input_base& input, float delta)
{
	if(actual==deseado)
	{
		escoger_nuevo_color();
	}

	std::stringstream ss;
	ss<<actual<<" "<<deseado;
	mut_debug(ss.str());
}

void Controlador_ejemplo::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::I_ESCAPE))
		{
			solicitar_cambio_estado(Director_estados::t_estados::INTRO);
		}
	}
}

void Controlador_ejemplo::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(actual.r, actual.g, actual.b, 255);
}

void Controlador_ejemplo::escoger_nuevo_color()
{
	Generador_int G(0, 255);
	deseado.r=G();
	deseado.g=G();
	deseado.b=G();
}

std::ostream &operator<<(std::ostream &os, Controlador_ejemplo::Color &c)
{
	os<<"["<<c.r<<","<<c.g<<","<<c.b<<"]";
	return os;

}
