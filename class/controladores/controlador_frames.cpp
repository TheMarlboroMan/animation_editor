#include "controlador_frames.h"
#include "../app/recursos.h"

Controlador_frames::Controlador_frames(Director_estados &DI, DLibV::Pantalla& pantalla, Tabla_sprites& ts)
	:Controlador_base(DI), 
	pantalla(pantalla), animacion(nullptr), tabla_sprites(ts), 
	listado(pantalla.acc_h()-y_inicio_lista, altura_linea),
	rep_listado(true),
	rep_seleccion_actual(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(y_inicio_lista, y_inicio_lista, pantalla.acc_w()-(2*y_inicio_lista), altura_linea),
			64, 64, 192),
	rep_animacion(DLibV::Gestor_texturas::obtener(10)),
	tiempo_pulsado(0.0f),
	indice_frame_nuevo(1)
{
	rep_listado.establecer_posicion(y_inicio_lista, y_inicio_lista),
	rep_animacion.establecer_posicion(pantalla.acc_w() / 2, y_inicio_lista);
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

		size_t indice_actual=listado.acc_indice_actual();
		bool recalcular=false;

		if(input.es_input_down(Input::I_RE_PAG))
		{
			recalcular=listado.cambiar_pagina(-1);
		}
		else if(input.es_input_down(Input::I_AV_PAG))
		{
			recalcular=listado.cambiar_pagina(1);
		}
		else if(input.es_input_down(Input::I_ABAJO))
		{
			recalcular=listado.cambiar_item(1);
		}
		else if(input.es_input_down(Input::I_ARRIBA))
		{
			recalcular=listado.cambiar_item(-1);
		}
		else if(input.es_input_down(Input::I_TAB) && indice_actual)
		{
			animacion->intercambiar_frames(indice_actual, indice_actual-1);
			recalcular=true;				
		}
		else if(input.es_input_down(Input::I_BORRAR) && animacion->size() > 1)
		{
			animacion->eliminar_frame(indice_actual);
			if(indice_actual) listado.cambiar_item(-1);
			recalcular=true;
		}
/*		else if(input.es_input_down(Input::I_ENTER))
		{
			auto frame=tabla_sprites.obtener(indice_frame_nuevo);
			if(frame)
			{
				animacion->modificar_frame(indice_actual, frame);
				recalcular=true;
			}
		}
*/
		else if(input.es_input_down(Input::I_ESPACIO))
		{
			solicitar_cambio_estado(Director_estados::t_estados::selector);
			return;
		}
		else if(input.es_input_down(Input::I_INSERTAR))
		{	
			auto frame=tabla_sprites.obtener(indice_frame_nuevo);
			if(frame)
			{
				animacion->insertar_frame(frame, 100, indice_actual);
				recalcular=true;
			}
		}
		else if(input.es_input_down(Input::I_MAS) || input.es_input_down(Input::I_MENOS))
		{
			int dir=input.es_input_pulsado(Input::I_MAS) ? 1 : -1;
			cambiar_duracion_frame(dir, 1.0f);
			recalcular=true;
		}
		else if(input.es_input_pulsado(Input::I_MAS) || input.es_input_pulsado(Input::I_MENOS))
		{
			tiempo_pulsado+=delta;
			if(tiempo_pulsado > 0.7f)
			{
				int dir=input.es_input_pulsado(Input::I_MAS) ? 1 : -1;
				cambiar_duracion_frame(dir, 2.0f);
				recalcular=true;
			}
		}
		else 
		{
			tiempo_pulsado=0.0f;
		}

		if(recalcular)
		{
			componer_lista();
			calcular_representacion_frame_actual();
			calcular_posicion_seleccion_actual();			
			componer_vista_lista();
		}
	}
}

void Controlador_frames::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
	rep_seleccion_actual.volcar(pantalla);
	rep_listado.volcar(pantalla);
	rep_animacion.volcar(pantalla);
}

void Controlador_frames::asignar_animacion(Animacion& anim)
{
	if(!anim)
	{
		throw std::runtime_error("ERROR: Animación a control de frames inválida");
	}

	listado.reiniciar_indice();
	animacion=&anim;
	calcular_representacion_frame_actual();
	componer_lista();
	componer_vista_lista();
}

void Controlador_frames::componer_lista()
{
	if(!animacion->size()) return;

	listado.clear();
	const size_t total=animacion->size();
	const auto& v=animacion->acc_lineas();
	std::stringstream ss;
	int i=0;
	for(auto& l : v)
	{
		ss<<i<<" / "<<total<<" : ("<<l.frame.x<<", "<<l.frame.y<<") ["<<l.duracion<<"s]";
		listado.insertar(item_listado{(size_t)i, ss.str()});
		ss.str("");
		++i;
	}
}

//TODO: Es común...
void Controlador_frames::componer_vista_lista()
{
	rep_listado.vaciar_grupo();
	const auto pagina=listado.obtener_pagina();
	using Texto=DLibV::Representacion_texto_auto_estatica;

	for(const auto& itemp : pagina)
	{
		Texto * txt=new Texto(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), "");
		txt->asignar(itemp.item.texto);
		txt->establecer_posicion(y_inicio_lista, itemp.y);
		rep_listado.insertar_representacion(txt);
	}
}

//TODO: Es común...
void Controlador_frames::calcular_posicion_seleccion_actual()
{
	int y=y_inicio_lista+(listado.linea_actual().y);	
	rep_seleccion_actual.establecer_posicion(y_inicio_lista, y);
}

void Controlador_frames::calcular_representacion_frame_actual()
{
	int w=pantalla.acc_w();
	auto f=animacion->obtener(listado.acc_indice_actual()).frame;
	if(f)
	{
		rep_animacion.establecer_recorte(f.x, f.y, f.w, f.h);
		rep_animacion.establecer_posicion(w - (w / 4), y_inicio_lista, f.w, f.h);
	}
}

void Controlador_frames::cambiar_duracion_frame(int val, float mult)
{
	auto& f=animacion->obtener(listado.acc_indice_actual());
	f.duracion+=0.010f * (float)val * mult;
	if(f.duracion < 0.010f) f.duracion=0.010f;
	animacion->reajustar_tiempo_frames();	//Esto es mal diseño... Debería cambiar sólo :D.
}


//Asigna desde el bootstrap.
void Controlador_frames::asignar_frame(size_t indice)
{
	indice_frame_nuevo=indice;

	auto frame=tabla_sprites.obtener(indice_frame_nuevo);
	if(frame)
	{
		animacion->modificar_frame(listado.acc_indice_actual(), frame);
		componer_lista();
		calcular_representacion_frame_actual();
		calcular_posicion_seleccion_actual();			
		componer_vista_lista();
	}
}
