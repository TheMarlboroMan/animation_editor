#include "controlador_selector.h"
#include "../app/recursos.h"

Controlador_selector::Controlador_selector(Director_estados &DI, DLibV::Pantalla& pantalla, Tabla_sprites& ts)
	:Controlador_base(DI),
	pantalla(pantalla), tabla_sprites(ts), 
	//TODO...
	listado(400, 400, dim_item_lista, dim_item_lista),
	rep_listado(true),
	rep_seleccion_actual(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, dim_item_lista, dim_item_lista),
			64, 64, 192),
	indice_frame(0)
{
	rep_listado.establecer_posicion(inicio_lista, inicio_lista);
	rep_seleccion_actual.establecer_alpha(128);
	componer_lista();
	componer_vista_lista();
	calcular_posicion_seleccion_actual();
	indice_frame=listado.item_actual().indice;
}

void Controlador_selector::preloop(Input_base& input, float delta)
{

}

void Controlador_selector::postloop(Input_base& input, float delta)
{

}

void Controlador_selector::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		if(input.es_input_down(Input::I_ESCAPE))
		{
			solicitar_cambio_estado(Director_estados::t_estados::frames);
			return;
		}

		bool recalcular=false;

		if(input.es_input_down(Input::I_RE_PAG))
		{
			recalcular=listado.cambiar_pagina(-1);
		}
		else if(input.es_input_down(Input::I_AV_PAG))
		{
			recalcular=listado.cambiar_pagina(1);
		}
		else if(input.es_input_down(Input::I_DERECHA))
		{
			recalcular=listado.cambiar_item(1);
		}
		else if(input.es_input_down(Input::I_IZQUIERDA))
		{
			recalcular=listado.cambiar_item(-1);
		}
		else if(input.es_input_down(Input::I_ABAJO))
		{
			recalcular=listado.cambiar_fila(1);
		}
		else if(input.es_input_down(Input::I_ARRIBA))
		{
			recalcular=listado.cambiar_fila(-1);
		}
		else if(input.es_input_down(Input::I_ESPACIO))
		{	
			indice_frame=listado.item_actual().indice;
			solicitar_cambio_estado(Director_estados::t_estados::frames);
			return;
		}

		if(recalcular)
		{
			componer_lista();
			calcular_posicion_seleccion_actual();			
			componer_vista_lista();
		}
	}
}

void Controlador_selector::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
	rep_listado.volcar(pantalla);
	rep_seleccion_actual.volcar(pantalla);
}

void Controlador_selector::componer_lista()
{
	listado.clear();
	const auto& frames=tabla_sprites.acc_sprites();
	for(auto& par : frames) listado.insertar(item_listado{par.first, par.second});
}

void Controlador_selector::componer_vista_lista()
{
	rep_listado.vaciar_grupo();

	const auto pagina=listado.obtener_pagina();
	using BMP=DLibV::Representacion_bitmap_estatica;

	size_t wl=listado.acc_w_item();
	size_t hl=listado.acc_h_item();

	for(const auto& itemp : pagina)
	{
		const auto& frame=itemp.item.frame;
//		int wr=std::max(wl, frame.w);
//		int hr=std::max(hl, frame.h);
		int wr=wl > frame.w ? wl : frame.w;
		int hr=hl > frame.h ? hl : frame.h;
		BMP * bmp=new BMP(DLibV::Gestor_texturas::obtener(10));
		bmp->establecer_recorte(frame.x, frame.y, frame.w, frame.h);
		bmp->establecer_posicion(itemp.x, itemp.y, wr, hr);
		rep_listado.insertar_representacion(bmp);
	}
}

void Controlador_selector::calcular_posicion_seleccion_actual()
{
	const auto& i=listado.linea_actual();
	rep_seleccion_actual.establecer_posicion(inicio_lista+i.x, inicio_lista+i.y);
}
