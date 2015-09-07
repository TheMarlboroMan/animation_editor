#include "controlador_animaciones.h"
#include "../app/recursos.h"

Controlador_animaciones::Controlador_animaciones(Director_estados &DI, DLibV::Pantalla& pantalla, Tabla_animaciones& tabla, const std::string nombre_fichero)
	:Controlador_base(DI),
	pantalla(pantalla),
	rep_listado(true),
	rep_seleccion_actual(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(y_inicio_lista, y_inicio_lista, pantalla.acc_w()-(2*y_inicio_lista), altura_linea),
			64, 64, 192),
	rep_animacion(DLibV::Gestor_texturas::obtener(10)),
	nombre_fichero(nombre_fichero),
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
		bool recalcular=false;

		if(!input.es_input_texto_activo())
		{
			if(input.es_input_down(Input::I_ESPACIO) && tabla_animaciones.size())
			{
				solicitar_cambio_estado(Director_estados::t_estados::frames);
				return;
			}
			else if(input.es_input_down(Input::I_RE_PAG))
			{
				recalcular=listado.cambiar_pagina(-1);
			}
			else if(input.es_input_down(Input::I_AV_PAG))
			{
				recalcular=listado.cambiar_pagina(1);
			}
			if(input.es_input_down(Input::I_ABAJO))
			{
				recalcular=listado.cambiar_item(1);
			}
			else if(input.es_input_down(Input::I_ARRIBA))
			{
				recalcular=listado.cambiar_item(-1);
			}
			else if(input.es_input_down(Input::I_INSERTAR))
			{
				tabla_animaciones.crear_animacion("Nueva animación");
				componer_lista();
				recalcular=true;
				//TODO: Establecer el índice a la nueva?.
			}
			else if(input.es_input_down(Input::I_BORRAR))
			{
				tabla_animaciones.eliminar_animacion(listado.item_actual().indice);
				componer_lista();
				recalcular=true;
				//TODO: Ojo, al borrar el último deberíamos cambiar un item para atrás.
			}
			else if(input.es_input_down(Input::I_TAB))
			{
				if(tabla_animaciones.size() > 1 && listado.acc_indice_actual() > 0)
				{
					size_t indice_actual=listado.item_actual().indice;
					size_t anterior=listado.item(listado.acc_indice_actual()-1).indice;
					tabla_animaciones.intercambiar_animaciones(indice_actual, anterior);
					componer_lista();
					recalcular=true;
				}
			}
			else if(input.es_input_down(Input::I_ENTER) && tabla_animaciones.size())
			{
				input.iniciar_input_texto();
				rep_seleccion_actual.mut_rgb(192, 64, 64);
			}
			else if(input.es_input_down(Input::I_GUARDAR) && tabla_animaciones.size())
			{
				guardar();
			}
		}
		else
		{
			if(input.es_input_down(Input::I_ENTER))
			{
				input.finalizar_input_texto();
				rep_seleccion_actual.mut_rgb(64, 64, 192);
			}
			else if(input.hay_input_texto())
			{
				tabla_animaciones.obtener(listado.item_actual().indice).mut_nombre(input.acc_input_texto());
				componer_lista();
				recalcular=true;
			}
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
	auto f=animacion.copia_para_tiempo_animacion(tiempo).frame;
	if(f)
	{
		rep_animacion.establecer_recorte(f.x, f.y, f.w, f.h);
		rep_animacion.establecer_posicion(pantalla.acc_w() - (pantalla.acc_w() / 4), y_inicio_lista, f.w, f.h);
	}
	else
	{
		rep_animacion.establecer_recorte(0, 0, 0, 0);
		rep_animacion.establecer_posicion(0, 0, 0, 0);
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
	using Texto=DLibV::Representacion_texto_auto_estatica;

	for(const auto& itemp : pagina)
	{
		Texto * txt=new Texto(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), "");
		txt->asignar(itemp.item.texto);
		txt->establecer_posicion(y_inicio_lista, itemp.y);
		rep_listado.insertar_representacion(txt);
	}
}

void Controlador_animaciones::calcular_posicion_seleccion_actual()
{
	int y=y_inicio_lista+(listado.linea_actual().y);	
	rep_seleccion_actual.establecer_posicion(y_inicio_lista, y);
}

void Controlador_animaciones::calcular_animacion_actual()
{
	if(!tabla_animaciones.size()) return;
	animacion=tabla_animaciones.obtener_copia(listado.item_actual().indice);
}

void Controlador_animaciones::guardar()
{
	std::ofstream fichero(nombre_fichero);

	if(!fichero)
	{
		LOG<<"ERROR: Imposible guardar en fichero "<<nombre_fichero<<std::endl;
	}
	else
	{
		const auto& animaciones=tabla_animaciones.acc_animaciones();
		for(const auto& par: animaciones)
		{
			const auto& animacion=par.second;
			fichero<<"*"<<animacion.acc_nombre()<<"\n"<<
				"!"<<par.first<<"\n";

			const auto& lineas=animacion.acc_lineas();
			for(const auto& linea : lineas)
			{
				fichero<<(linea.duracion * 1000.0f)<<"\t"<<33<<"\n";
			}
		}

		fichero.close();
		LOG<<"Cambios guardados."<<std::endl;
	}
}
