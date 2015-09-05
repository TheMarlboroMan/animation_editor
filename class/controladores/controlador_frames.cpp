#include "controlador_frames.h"
#include "../app/recursos.h"

Controlador_frames::Controlador_frames(Director_estados &DI, DLibV::Pantalla& pantalla, Tabla_sprites& ts)
	:Controlador_base(DI), animacion(nullptr), tabla_sprites(ts), 
	rep_txt(pantalla.acc_renderer(), DLibV::Gestor_superficies::obtener(Recursos_graficos::RS_FUENTE_BASE), ""),
	rep_seleccion_actual(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(y_inicio_lista, y_inicio_lista, pantalla.acc_w()-(2*y_inicio_lista), altura_linea),
			64, 64, 192),
	rep_animacion(DLibV::Gestor_texturas::obtener(10)),
	w(pantalla.acc_w()),
	tiempo_pulsado(0.0f),
	indice_frame_nuevo(1),
	estructura_paginacion(altura_linea, 0, pantalla.acc_h()-y_inicio_lista)
{
	rep_txt.establecer_posicion(y_inicio_lista, y_inicio_lista);
	rep_txt.mut_interlineado(4);
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

//		size_t pag=estructura_paginacion.acc_pagina_actual();
		size_t indice_actual=estructura_paginacion.acc_indice_actual();
		bool recalcular=false;

		if(input.es_input_down(Input::I_RE_PAG))
		{
			recalcular=estructura_paginacion.cambiar_pagina(-1);
		}
		else if(input.es_input_down(Input::I_AV_PAG))
		{
			recalcular=estructura_paginacion.cambiar_pagina(1);
		}
		else if(input.es_input_down(Input::I_ABAJO))
		{
			recalcular=estructura_paginacion.cambiar_item(1);
		}
		else if(input.es_input_down(Input::I_ARRIBA))
		{
			recalcular=estructura_paginacion.cambiar_item(-1);
		}
		else if(input.es_input_down(Input::I_TAB) && indice_actual)
		{
			animacion->intercambiar_frames(indice_actual, indice_actual-1);
			recalcular=true;				
		}
		else if(input.es_input_down(Input::I_BORRAR) && animacion->size() > 1)
		{
			animacion->eliminar_frame(indice_actual);
			if(indice_actual) estructura_paginacion.cambiar_item(-1);
			recalcular=true;
		}
		else if(input.es_input_down(Input::I_ENTER))
		{
/*
TODO: Crear clase template listado paginado, con métodos clear, que se pueda recomponer
and shit. Quizás que contenga información posicional también acerca de la posición
actual, por ejemplo, de forma que no haya que calcularla fuera.
Sería algo como
template<typename T>
listado_lineal
{
	estructura_paginacion;
	std::vector<Linea_listado<T>>;

	passthroughs estructura_paginacion;
	passthroughs vector;
}
*/

			//TODO: Tener dos clases distintas para animación y 
			//frames: una editable y otra no editable, para 
			//importarla a otros proyectos.

			//TODO: Tener un controlador sólo para seleccionar el frame
			//actual y que sea accesible desde este controller al pulsar
			//alguna tecla. Pasar el valor del frame actual a este otro
			//controller al volver.

			auto frame=tabla_sprites.obtener(indice_frame_nuevo);
			if(frame)
			{
				animacion->modificar_frame(indice_actual, frame);
				recalcular=true;
			}
		}
		else if(input.es_input_down(Input::I_ESPACIO))
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
	rep_txt.volcar(pantalla);
	rep_animacion.volcar(pantalla);
}

void Controlador_frames::asignar_animacion(Animacion& anim)
{
	if(!anim)
	{
		throw std::runtime_error("ERROR: Animación a control de frames inválida");
	}

	estructura_paginacion.reiniciar_indice();
	animacion=&anim;
	calcular_representacion_frame_actual();
	componer_lista();
	componer_vista_lista();
}

void Controlador_frames::componer_lista()
{
	lineas_listado.clear();
	const size_t total=animacion->size();
	estructura_paginacion.establecer_total_elementos(total);
	const auto& v=animacion->acc_lineas();
	std::stringstream ss;
	int i=0;
	for(auto& l : v)
	{
		ss<<i<<" / "<<total<<" : ("<<l.frame.x<<", "<<l.frame.y<<") ["<<l.duracion<<"s]";
		lineas_listado.push_back(Linea_listado<std::string>(i, ss.str() ) );
		ss.str("");
		++i;
	}
}

//TODO: Es común...
void Controlador_frames::componer_vista_lista()
{
	size_t reg=estructura_paginacion.acc_registros_por_pagina();

	auto 	ini=std::begin(lineas_listado)+(estructura_paginacion.acc_pagina_actual() * reg),
		fin=ini + reg;
	std::stringstream ss;

	while(ini < std::end(lineas_listado) && ini < fin)
	{
		ss<<ini->acc_valor()<<"\n";
		++ini;
	}

	rep_txt.asignar(ss.str());
}

//TODO: Es común...
void Controlador_frames::calcular_posicion_seleccion_actual()
{
	size_t indice=estructura_paginacion.acc_indice_actual() % estructura_paginacion.acc_registros_por_pagina();
	rep_seleccion_actual.establecer_posicion(y_inicio_lista, y_inicio_lista+(indice * altura_linea));
}

void Controlador_frames::calcular_representacion_frame_actual()
{
	auto f=animacion->obtener(estructura_paginacion.acc_indice_actual()).frame;
	if(f)
	{
		rep_animacion.establecer_recorte(f.x, f.y, f.w, f.h);
		rep_animacion.establecer_posicion(w - (w / 4), y_inicio_lista, f.w, f.h);
	}
}

void Controlador_frames::cambiar_duracion_frame(int val, float mult)
{
	auto& f=animacion->obtener(estructura_paginacion.acc_indice_actual());
	f.duracion+=0.010f * (float)val * mult;
	if(f.duracion < 0.010f) f.duracion=0.010f;
	animacion->reajustar_tiempo_frames();	//Esto es mal diseño... Debería cambiar sólo :D.
}
