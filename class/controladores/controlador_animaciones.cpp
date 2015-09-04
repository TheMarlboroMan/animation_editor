#include "controlador_animaciones.h"
#include "../app/recursos.h"

Controlador_animaciones::Controlador_animaciones(Director_estados &DI, DLibV::Pantalla& pantalla, Tabla_animaciones& tabla)
	:Controlador_base(DI),
	rep_txt(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), ""),
	rep_seleccion_animacion_actual(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(y_inicio_lista, y_inicio_lista, pantalla.acc_w()-(2*y_inicio_lista), altura_linea),
			64, 64, 192),
	rep_animacion(DLibV::Gestor_texturas::obtener(10)),
	tabla_animaciones(tabla),
	registros_por_pagina(0), pagina_actual(0), total_paginas(0), animacion_actual(0),
	tiempo(0.0f)
{
	rep_txt.establecer_posicion(16, 16);
	rep_txt.mut_interlineado(4);
	rep_animacion.establecer_posicion(y_inicio_lista, pantalla.acc_w() / 2);

	calcular_informacion_paginas(pantalla.acc_h());
	componer_lista();
	componer_vista_lista();
	calcular_animacion_actual();
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
	tiempo+=delta;

	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		abandonar_aplicacion();
	}
	else
	{
		//Control de paginación...
		if(input.es_input_down(Input::I_RE_PAG))
		{
			if(pagina_actual > 0) 
			{
				cambiar_pagina(-1);
				animacion_actual-=registros_por_pagina;
				calcular_posicion_seleccion_animacion_actual();
				calcular_animacion_actual();
			}
		}
		else if(input.es_input_down(Input::I_AV_PAG))
		{
			if(pagina_actual + 1 <= total_paginas) 
			{
				cambiar_pagina(1);
				animacion_actual+=registros_por_pagina;
				calcular_posicion_seleccion_animacion_actual();
				calcular_animacion_actual();
			}
		}

		//Control de selección...
		if(input.es_input_down(Input::I_ABAJO))
		{
			if(animacion_actual+1 < tabla_animaciones.size())
			{
				cambiar_animacion(1);
				calcular_posicion_seleccion_animacion_actual();
				calcular_animacion_actual();
			}
		}
		else if(input.es_input_down(Input::I_ARRIBA))
		{
			if(animacion_actual > 0) 
			{
				cambiar_animacion(-1);
				calcular_posicion_seleccion_animacion_actual();
				calcular_animacion_actual();
			}
		}
	}
}

void Controlador_animaciones::cambiar_animacion(int val)
{
	animacion_actual+=val;
	
	//Detectar si vamos a cambiar de página.
	size_t indice=animacion_actual % (registros_por_pagina+1);
	
	if(indice==registros_por_pagina)
	{
		cambiar_pagina(val);
	}
}

void Controlador_animaciones::cambiar_pagina(int val)
{
	pagina_actual+=val;
	componer_vista_lista();
}

void Controlador_animaciones::dibujar(DLibV::Pantalla& pantalla)
{
	auto f=animacion.obtener_para_tiempo_animacion(tiempo).frame;
	if(f)
	{
		rep_animacion.establecer_recorte(f.x, f.y, f.w, f.h);
		rep_animacion.establecer_posicion(y_inicio_lista, pantalla.acc_w() / 2, f.w, f.h);
	}

	pantalla.limpiar(0, 0, 0, 255);
	rep_seleccion_animacion_actual.volcar(pantalla);
	rep_txt.volcar(pantalla);
	rep_animacion.volcar(pantalla);
}

void Controlador_animaciones::componer_lista()
{
	if(! tabla_animaciones.size()) return;
	
	lineas_animaciones.clear();
	std::stringstream ss;

	const auto& animaciones=tabla_animaciones.acc_animaciones();
	for(const auto& par : animaciones)
	{
		const auto& an=par.second;

		ss<<par.first<<" : "<<an.acc_nombre()<<" ["<<an.acc_duracion_total()<<"ms]";
		lineas_animaciones.push_back( Linea_listado(par.first, ss.str() ) );
		ss.str("");
	}
}

void Controlador_animaciones::componer_vista_lista()
{
	auto 	ini=std::begin(lineas_animaciones)+(pagina_actual * registros_por_pagina),
		fin=ini + registros_por_pagina;
	std::stringstream ss;

	while(ini < std::end(lineas_animaciones) && ini < fin)
	{
		ss<<ini->acc_texto()<<"\n";
		++ini;
	}

	rep_txt.asignar(ss.str());
}

void Controlador_animaciones::calcular_informacion_paginas(int h)
{
	h-=y_inicio_lista;
	registros_por_pagina=floor(h / altura_linea);
	total_paginas=ceil(tabla_animaciones.size() / registros_por_pagina);
}

void Controlador_animaciones::calcular_posicion_seleccion_animacion_actual()
{
	size_t indice=animacion_actual % registros_por_pagina;
	rep_seleccion_animacion_actual.establecer_posicion(y_inicio_lista, y_inicio_lista+(indice * altura_linea));
}

void Controlador_animaciones::calcular_animacion_actual()
{	
	if(!tabla_animaciones.size()) return;
	animacion=tabla_animaciones.obtener(lineas_animaciones[animacion_actual].acc_indice_animacion());
}
