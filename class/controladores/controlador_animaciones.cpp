#include "controlador_animaciones.h"
#include "../app/recursos.h"

Controlador_animaciones::Controlador_animaciones(Director_estados &DI, DLibV::Pantalla& pantalla, Tabla_animaciones& tabla)
	:Controlador_base(DI),
	pantalla(pantalla),
	rep_seleccion_actual(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(y_inicio_lista, y_inicio_lista, pantalla.acc_w()-(2*y_inicio_lista), altura_linea),
			64, 64, 192),
	rep_animacion(DLibV::Gestor_texturas::obtener(10)),
	tabla_animaciones(tabla),
	listado(pantalla.acc_h()-y_inicio_lista, altura_linea),
	tiempo(0.0f)
{
	rep_listado.establecer_posicion(y_inicio_lista, y_inicio_lista),
	rep_animacion.establecer_posicion(pantalla.acc_w() - (pantalla.acc_w() / 4), y_inicio_lista);
	refrescar();
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

		bool recalcular=false;

		//Control de paginación...
		if(input.es_input_down(Input::I_RE_PAG))
		{
			recalcular=listado.cambiar_pagina(-1);
		}
		else if(input.es_input_down(Input::I_AV_PAG))
		{
			recalcular=listado.cambiar_pagina(1);
		}

		//Control de selección...
		if(input.es_input_down(Input::I_ABAJO))
		{
			recalcular=listado.cambiar_item(1);
		}
		else if(input.es_input_down(Input::I_ARRIBA))
		{
			recalcular=listado.cambiar_item(-1);
		}
		
		if(recalcular)
		{
			calcular_posicion_seleccion_actual();
			calcular_animacion_actual();

//			if(pag != listado.acc_pagina_actual())
//			{
			componer_vista_lista();
//			}
		}
	}
}

void Controlador_animaciones::dibujar(DLibV::Pantalla& pantalla)
{
	auto f=animacion.obtener_para_tiempo_animacion(tiempo).frame;
	if(f)
	{
		rep_animacion.establecer_recorte(f.x, f.y, f.w, f.h);
		rep_animacion.establecer_posicion(pantalla.acc_w() - (pantalla.acc_w() / 4), y_inicio_lista, f.w, f.h);
	}

	pantalla.limpiar(0, 0, 0, 255);
	rep_seleccion_actual.volcar(pantalla);
	rep_listado.volcar(pantalla);
	rep_animacion.volcar(pantalla);
}

void Controlador_animaciones::refrescar()
{
	componer_lista();
	componer_vista_lista();
	calcular_animacion_actual();
}

void Controlador_animaciones::componer_lista()
{
	if(! tabla_animaciones.size()) return;
	
	listado.clear();
	std::stringstream ss;

	const auto& animaciones=tabla_animaciones.acc_animaciones();
	for(const auto& par : animaciones)
	{
		const auto& an=par.second;
		ss<<par.first<<" : "<<an.acc_nombre()<<" ["<<an.size()<<" frames, "<<an.acc_duracion_total()<<"s]";
		listado.insertar(item_listado{par.first, ss.str()});
		ss.str("");
	}
}

void Controlador_animaciones::componer_vista_lista()
{
	rep_listado.vaciar_grupo();
	const auto pagina=listado.obtener_pagina();
	using DLibV::Representacion_texto_auto_estatica=Texto;

	for(const auto& item : pagina)
	{
		Texto * txt=new Texto(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), "");
		txt->asignar(item.item.acc_valor());
		txt->establecer_posicion(y_inicio_lista, item.y);
		rep_listado.insertar_representacion(txt);
	}
}

void Controlador_animaciones::calcular_posicion_seleccion_actual()
{
	//TODO... Posición...
//	size_t indice=listado.acc_indice_actual() % listado.acc_registros_por_pagina();
//	rep_seleccion_actual.establecer_posicion(y_inicio_lista, y_inicio_lista+(indice * altura_linea));
}

void Controlador_animaciones::calcular_animacion_actual()
{
	//TODO: Operador []... 	
//	if(!tabla_animaciones.size()) return;
//	animacion=tabla_animaciones.obtener(lineas_listado[listado.acc_indice_actual()].acc_indice());
}
