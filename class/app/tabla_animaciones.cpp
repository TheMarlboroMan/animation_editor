#include "tabla_animaciones.h"

Linea_animacion::operator bool() const
{
	return duracion && frame;
}

/////////////////////////////////////

Animacion::Animacion()
	:duracion_total(0.0f)
{

}
				
Animacion::operator bool() const
{
	return duracion_total || lineas.size(); // && nombre.size();
}

void Animacion::eliminar_frame(size_t f)
{
	auto it=std::begin(lineas)+f;
	if(it < std::end(lineas))
	{
		lineas.erase(it);
		reajustar_tiempo_frames();
	}
}

void Animacion::modificar_frame(size_t pos, const Frame_sprites& v)
{
	(std::begin(lineas)+pos)->frame=v;
}

const Linea_animacion& Animacion::obtener(size_t val) const
{
	return lineas.at(val);
}

Linea_animacion& Animacion::obtener(size_t val)
{
	return lineas.at(val);
}

void Animacion::insertar_frame(const Frame_sprites& v, int d)
{
	float dur=(float)d / 1000.f;
	duracion_total+=dur;
	lineas.push_back(Linea_animacion{dur, 0.0f, v});
	reajustar_tiempo_frames();
}

void Animacion::insertar_frame(const Frame_sprites& v, int d, size_t pos)
{
	float dur=(float)d / 1000.f;
	duracion_total+=dur;
	lineas.insert(std::begin(lineas)+pos, Linea_animacion{dur, 0.0f, v});
	reajustar_tiempo_frames();
}

/**
* Ajusta el valor "momento_aparición" de cada uno de los frames con respecto
* al tiempo general de la aplicación.
*/

void Animacion::reajustar_tiempo_frames()
{
	duracion_total=0;
	for(auto& l: lineas)
	{
		duracion_total+=l.duracion;
		l.momento_aparicion=duracion_total;
	}
}

const Linea_animacion& Animacion::obtener_para_tiempo_animacion(float t)
{
	float transformado=fmod(t, duracion_total);
	for(const Linea_animacion& fr : lineas)
	{
		if(transformado <= fr.momento_aparicion) return fr;
	}
	return lineas.at(0);
}

Linea_animacion Animacion::copia_para_tiempo_animacion(float t)
{
	if(!lineas.size())
	{
		return Linea_animacion();
	}
	else
	{
		float transformado=fmod(t, duracion_total);
		for(const Linea_animacion& fr : lineas)
		{
			if(transformado <= fr.momento_aparicion) return fr;
		}
		return lineas.at(0);
	}
}

void Animacion::intercambiar_frames(size_t indice_a, size_t indice_b)
{
	std::iter_swap(lineas.begin()+indice_a, lineas.begin()+indice_b);
	reajustar_tiempo_frames();	
}

/////////////////////////////////////

Tabla_animaciones::Tabla_animaciones(const Tabla_sprites& t)
	:tabla_sprites(t)
{	

}

Tabla_animaciones::Tabla_animaciones(const Tabla_sprites& t, const std::string& ruta)
	:tabla_sprites(t)
{	
	cargar(ruta);
}

void Tabla_animaciones::cargar(const std::string& ruta)
{
	DLibH::Lector_txt L(ruta, '#');

	if(!L)	
	{
		LOG<<"ERROR: Para Tabla_animaciones no se ha podido abrir el archivo "<<ruta<<std::endl;
	}
	else
	{
		std::string linea;
		const char inicio_titulo='*';
		const char inicio_cabecera='!';
		size_t id=0;
		Animacion animacion;

		auto insertar_anim=[this](Animacion animacion, size_t id)
		{
			animacion.reajustar_tiempo_frames();
			animaciones[id]=animacion;					
		};

		try
		{
			while(true)
			{
				linea=L.leer_linea();
				if(!L) 
				{	
					//Insertar la última animación...
					if(animacion) insertar_anim(animacion, id);
					break;
				}

				const char inicio=linea[0];
				switch(inicio)
				{
					case inicio_titulo: 
						if(animacion) insertar_anim(animacion, id);
						animacion=Animacion(); //Reset animación...
						interpretar_como_titulo(linea.substr(1), animacion); 
					break;
					case inicio_cabecera: 
						id=interpretar_como_cabecera(linea.substr(1)); 
					break;
					default: 
						interpretar_como_linea(linea, animacion); 
					break;
				}
			}
		}
		catch(std::runtime_error& e)
		{
			LOG<<e.what()<<" : Línea "<<L.obtener_numero_linea()<<" ["<<linea<<"]. Cancelando."<<std::endl;
			throw e;
		}
		catch(std::out_of_range& e)
		{
			LOG<<e.what()<<" : Línea "<<L.obtener_numero_linea()<<" ["<<linea<<"]. Cancelando."<<std::endl;
			throw e;
		}
	}
}

void Tabla_animaciones::interpretar_como_titulo(const std::string& linea, Animacion& animacion)
{
	auto titulo=linea;
	animacion.mut_nombre(linea);
}
	
size_t Tabla_animaciones::interpretar_como_cabecera(const std::string& linea)
{
	const char separador='\t';
	const std::vector<std::string> valores=DLibH::Herramientas::explotar(linea, separador);
	if(valores.size()==1)
	{
		int id=std::atoi(valores[0].c_str());
		return id;
	}
	else
	{
		throw std::runtime_error("Error al leer cabecera de animación.");
	}
}

void Tabla_animaciones::interpretar_como_linea(const std::string& linea, Animacion& animacion)
{
	const char separador='\t';
	std::vector<std::string> valores=DLibH::Herramientas::explotar(linea, separador);
	if(valores.size()==2)
	{
		int duracion=std::atoi(valores[0].c_str());
		int indice_frame=std::atoi(valores[1].c_str());

		try
		{
			const auto& frame=tabla_sprites.obtener(indice_frame);
			animacion.insertar_frame(frame, duracion);
		}
		catch(std::out_of_range& e)
		{
			LOG<<e.what()<<" : no se localiza el indice frame "<<indice_frame<<" para interpretar como línea"<<std::endl;
			throw e;
		}
	}
	else
	{
		throw std::runtime_error("Error al leer cabecera de animación.");
	}
}

const Animacion& Tabla_animaciones::obtener(size_t indice) const
{
	return animaciones.at(indice);
}

Animacion& Tabla_animaciones::obtener(size_t indice)
{
	return animaciones[indice];
}

Animacion Tabla_animaciones::obtener_copia(size_t indice)
{
	if(animaciones.count(indice)) return animaciones[indice];
	else return Animacion();
}

Animacion& Tabla_animaciones::crear_animacion(const std::string& nombre)
{
	size_t indice=animaciones.size() ? animaciones.rbegin()->first + 1 : 1;
	Animacion animacion;
	animacion.mut_nombre(nombre);
	//Insertar al menos un frame, para que sea válida.
	animacion.insertar_frame(tabla_sprites.acc_sprites().begin()->second, 100);
	animaciones[indice]=animacion;
	return animaciones.at(indice);
}

void Tabla_animaciones::eliminar_animacion(size_t i)
{
	animaciones.erase(i);
}

void Tabla_animaciones::intercambiar_animaciones(size_t indice_a, size_t indice_b)
{
	std::swap(animaciones[indice_a], animaciones[indice_b]);
}
