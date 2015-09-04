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
	estructura_paginacion(altura_linea, tabla_animaciones.size(), 0),
	tiempo(0.0f)
{
	rep_txt.establecer_posicion(16, 16);
	rep_txt.mut_interlineado(4);
	rep_animacion.establecer_posicion(y_inicio_lista, pantalla.acc_w() / 2);

	estructura_paginacion.establecer_altura(pantalla.acc_h()-y_inicio_lista);

	componer_lista();
	componer_vista_lista();
	calcular_animacion_actual();
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
		if(input.es_input_down(Input::I_ENTER) && tabla_animaciones.size())
		{
			solicitar_cambio_estado(Director_estados::t_estados::frames);
			return;
		}

		size_t pag=estructura_paginacion.acc_pagina_actual();
		bool recalcular=false;

		//Control de paginación...
		if(input.es_input_down(Input::I_RE_PAG))
		{
			recalcular=estructura_paginacion.cambiar_pagina(-1);
		}
		else if(input.es_input_down(Input::I_AV_PAG))
		{
			recalcular=estructura_paginacion.cambiar_pagina(1);
		}

		//Control de selección...
		if(input.es_input_down(Input::I_ABAJO))
		{
			recalcular=estructura_paginacion.cambiar_item(1);
		}
		else if(input.es_input_down(Input::I_ARRIBA))
		{
			recalcular=estructura_paginacion.cambiar_item(-1);
		}
		
		if(recalcular)
		{
			calcular_posicion_seleccion_animacion_actual();
			calcular_animacion_actual();

			if(pag != estructura_paginacion.acc_pagina_actual())
			{
				componer_vista_lista();
			}
		}
	}
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
	size_t reg=estructura_paginacion.acc_registros_por_pagina();

	auto 	ini=std::begin(lineas_animaciones)+(estructura_paginacion.acc_pagina_actual() * reg),
		fin=ini + reg;
	std::stringstream ss;

	while(ini < std::end(lineas_animaciones) && ini < fin)
	{
		ss<<ini->acc_texto()<<"\n";
		++ini;
	}

	rep_txt.asignar(ss.str());
}

void Controlador_animaciones::calcular_posicion_seleccion_animacion_actual()
{
	size_t indice=estructura_paginacion.acc_indice_actual() % estructura_paginacion.acc_registros_por_pagina();
	rep_seleccion_animacion_actual.establecer_posicion(y_inicio_lista, y_inicio_lista+(indice * altura_linea));
}

void Controlador_animaciones::calcular_animacion_actual()
{	
	if(!tabla_animaciones.size()) return;
	animacion=tabla_animaciones.obtener(lineas_animaciones[estructura_paginacion.acc_indice_actual()].acc_indice_animacion());
}
