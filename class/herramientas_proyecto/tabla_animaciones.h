#ifndef TABLA_ANIMACIONES_H
#define TABLA_ANIMACIONES_H

#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <herramientas/lector_txt/lector_txt.h>
#include "tabla_sprites.h"


struct Linea_animacion
{
	public:

					explicit operator bool() const;
	int 				duracion;
	Frame_sprites 			frame;
};

class Animacion
{
	public:		
					Animacion();
					explicit operator bool() const;	
	const Linea_animacion&		obtener(size_t) const;
	void				insertar_frame(const Frame_sprites& v, int);

	size_t				size() const {return lineas.size();}
	const std::string&		acc_nombre() const {return nombre;}
	int				acc_duracion_total() const {return duracion_total;}
	void				mut_nombre(const std::string&v) {nombre=v;}

	private:

	std::string			nombre;
	std::vector<Linea_animacion>	lineas;
	int				duracion_total;

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

	private:

	void				interpretar_como_titulo(const std::string&, Animacion&);
	size_t				interpretar_como_cabecera(const std::string&);
	void				interpretar_como_linea(const std::string&, Animacion&);

	const Tabla_sprites&		tabla_sprites;	
	std::map<size_t, Animacion>	animaciones;
};

#endif
