#ifndef TABLA_ANIMACIONES_H
#define TABLA_ANIMACIONES_H

#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iterator>

#include <herramientas/lector_txt/lector_txt.h>
#include "tabla_sprites.h"


struct Linea_animacion
{
	public:

					explicit operator bool() const;
	float 				duracion;			//Cuánto dura.
	float				momento_aparicion;			//Cuál es el último momento de su duración dentro de toda la animación.
	Frame_sprites 			frame;
};

class Animacion
{
	public:
					Animacion();
					explicit operator bool() const;
	const Linea_animacion&		obtener(size_t) const;
	Linea_animacion&		obtener(size_t);
	const Linea_animacion&		obtener_para_tiempo_animacion(float);
	void				insertar_frame(const Frame_sprites& v, int);
	void				reajustar_tiempo_frames();
	void				intercambiar_frames(size_t, size_t);
	void				eliminar_frame(size_t);

	bool				es_estatica() const {return lineas.size()==1;}
	size_t				size() const {return lineas.size();}
	const std::string&		acc_nombre() const {return nombre;}
	float				acc_duracion_total() const {return duracion_total;}
	void				mut_nombre(const std::string&v) {nombre=v;}
	const std::vector<Linea_animacion>& acc_lineas() const {return lineas;}

	private:

	std::string			nombre;
	std::vector<Linea_animacion>	lineas;
	float				duracion_total;

};

class Tabla_animaciones
{
	public:
					Tabla_animaciones(const Tabla_sprites&);
					Tabla_animaciones(const Tabla_sprites&, const std::string&);

	void 				cargar(const std::string&);
	
	const Animacion& 		obtener(size_t) const;
	Animacion& 			obtener(size_t);
	Animacion 			obtener_copia(size_t);

	Animacion&			crear_animacion(const std::string&);
	size_t				size() {return animaciones.size();}
	size_t				obtener_siguiente_indice() const;
	size_t				obtener_primer_indice() const;
	size_t				obtener_ultimo_indice() const;
	bool				existe_indice(size_t v) const {return animaciones.count(v);}
	std::vector<size_t>		obtener_vector_claves() const;
	const std::map<size_t, Animacion>& acc_animaciones() const {return animaciones;}
	const Tabla_sprites&		acc_tabla_sprites() const {return tabla_sprites;}

	private:

	void				interpretar_como_titulo(const std::string&, Animacion&);
	size_t				interpretar_como_cabecera(const std::string&);
	void				interpretar_como_linea(const std::string&, Animacion&);

	const Tabla_sprites&		tabla_sprites;	
	std::map<size_t, Animacion>	animaciones;
};

#endif
