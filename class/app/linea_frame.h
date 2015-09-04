#ifndef LINEA_FRAME_H
#define LINEA_FRAME_H

#include <string>

class Linea_frame
{
	public:

					Linea_frame(float, size_t);
	float				acc_duracion() {return duracion;}
	size_t				acc_indice_frame() {return indice_frame;}
	
	private:

	float 				duracion;
	size_t				indice_frame;

};

#endif
