#include <fstream>
#include "bootstrap_aplicacion.h"

#include "../class/controladores/controlador_intro.h"
#include "../class/controladores/controlador_ejemplo.h"

using namespace App;

void App::loop_aplicacion(Kernel_app& kernel)
{
	try
	{
		auto& ruta_out=kernel.acc_ruta_destino();
		auto& ruta_hoja=kernel.acc_ruta_hoja_sprites();

		//Comprobar existencia de la hoja de sprites...
		std::ifstream test_hoja(ruta_hoja);
		if(!test_hoja.is_open()) throw std::runtime_error("No se localiza el fichero de hoja de sprites");

		//Declaraciones de herramientas externas.
		Director_estados DI;

		//Controladores e interfaces.
		Controlador_intro C_I(DI, kernel.acc_pantalla());
		Controlador_ejemplo C_E(DI);

		Interface_controlador * IC=&C_I;

		//Loop principal.
		while(kernel.loop(*IC))
		{
			if(DI.es_cambio_estado())
			{
				switch(DI.acc_estado_actual())
				{
					case Director_estados::t_estados::INTRO: break;
					case Director_estados::t_estados::EJEMPLO: break;
				}

				switch(DI.acc_estado_deseado())
				{
					case Director_estados::t_estados::INTRO: IC=&C_I; break;
					case Director_estados::t_estados::EJEMPLO: IC=&C_E; break;
				}

				DI.confirmar_cambio_estado();
			}
		};
	}
	catch(std::runtime_error& e)
	{
		std::cout<<"ERROR: "<<e.what()<<std::endl;
	}
}
