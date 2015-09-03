#include <fstream>
#include "bootstrap_aplicacion.h"

#include "../class/controladores/controlador_animaciones.h"
#include "../class/herramientas_proyecto/tabla_animaciones.h"

using namespace App;

void App::loop_aplicacion(Kernel_app& kernel)
{
	try
	{
		auto& ruta_out=kernel.acc_ruta_destino();
		auto& ruta_hoja=kernel.acc_ruta_hoja_sprites();

		//Comprobar existencia de la hoja de sprites e iniciar la información...
		std::ifstream test_hoja(ruta_hoja);
		if(!test_hoja.is_open()) throw std::runtime_error("No se localiza el fichero de hoja de sprites");
		Tabla_sprites tabla_sprites(ruta_hoja);

		//Igual, pero con el output...
		Tabla_animaciones tabla_animaciones(tabla_sprites);
		std::ifstream test_out(ruta_out);
		if(test_out.is_open()) 
		{
			tabla_animaciones.cargar(ruta_out);
/*
			const auto& anim=tabla_animaciones.obtener(1);
			LOG<<anim.acc_nombre()<<", "<<anim.acc_duracion_total()<<std::endl;

			size_t i=0;
			while(i < 4)
			{
				const auto& f=anim.obtener(i);
				LOG<<"FRAME "<<i<<": "<<f.duracion<<" -> "<<f.frame.x<<" "<<f.frame.y<<std::endl;
				++i;
			}
			std::cout<<tabla_animaciones.obtener_siguiente_indice()<<std::endl;
*/
		}

		//TODO: Usar "obtener_vector_claves" para sacar las posibles claves e irlas pintando
		//de un modo controlado. Dejar un hueco para pintar la animación en si.

		//Declaraciones de herramientas externas.
		Director_estados director_estados;

		//Controladores e interfaces.
		Controlador_animaciones controlador_animaciones(director_estados, kernel.acc_pantalla());
		Interface_controlador * interface_controlador=&controlador_animaciones;

		//Loop principal.
		while(kernel.loop(*interface_controlador))
		{
			if(director_estados.es_cambio_estado())
			{
				switch(director_estados.acc_estado_actual())
				{
					case Director_estados::t_estados::animaciones: break;
				}

				switch(director_estados.acc_estado_deseado())
				{
					case Director_estados::t_estados::animaciones: interface_controlador=&controlador_animaciones; break;
				}

				director_estados.confirmar_cambio_estado();
			}
		};
	}
	catch(std::runtime_error& e)
	{
		std::cout<<"ERROR: "<<e.what()<<std::endl;
	}
}
