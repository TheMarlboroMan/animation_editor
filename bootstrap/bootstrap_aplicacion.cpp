#include <fstream>
#include "bootstrap_aplicacion.h"

#include "../class/controladores/controlador_animaciones.h"
#include "../class/controladores/controlador_frames.h"
#include "../class/controladores/controlador_selector.h"
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
		if(test_out.is_open()) tabla_animaciones.cargar(ruta_out);

		//Declaraciones de herramientas externas.
		Director_estados director_estados;

		//Controladores e interfaces.
		Controlador_animaciones controlador_animaciones(director_estados, kernel.acc_pantalla(), tabla_animaciones);
		Controlador_frames controlador_frames(director_estados, kernel.acc_pantalla(), tabla_sprites);
		Controlador_selector controlador_selector(director_estados, kernel.acc_pantalla(), tabla_sprites);
		Interface_controlador * interface_controlador=&controlador_animaciones;

		//Loop principal.
		while(kernel.loop(*interface_controlador))
		{
			if(director_estados.es_cambio_estado())
			{
				switch(director_estados.acc_estado_actual())
				{
					case Director_estados::t_estados::animaciones: break;
					case Director_estados::t_estados::frames: break;
					case Director_estados::t_estados::selector: 
						controlador_frames.asignar_frame(controlador_selector.acc_indice_frame());			
					break;
				}

				switch(director_estados.acc_estado_deseado())
				{
					case Director_estados::t_estados::animaciones: 
						controlador_animaciones.refrescar();
						interface_controlador=&controlador_animaciones; 
					break;
					case Director_estados::t_estados::frames:
						controlador_frames.asignar_animacion(tabla_animaciones.obtener(controlador_animaciones.acc_indice_animacion_actual()));
						interface_controlador=&controlador_frames;
					break;
					case Director_estados::t_estados::selector:
						interface_controlador=&controlador_selector;
					break;
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
