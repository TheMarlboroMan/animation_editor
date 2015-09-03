#ifndef CONTROLADOR_EJEMPLO
#define CONTROLADOR_EJEMPLO

#include "controlador_base.h"
#include "../herramientas_proyecto/generador_numeros.h"

class Controlador_ejemplo:public Controlador_base
{
	//////////////
	//Definiciones
	public:
	
	struct Color
	{
		unsigned int r;
		unsigned int g;
		unsigned int b;
		Color(unsigned int pr, unsigned int pg, unsigned int pb)
			:r(pr), g(pg), b(pb)
		{}

		bool operator!=(const Color& o) const	
		{
			return r!=o.r || g!=o.g || b!=o.b;
		}	

		bool operator==(const Color& o) const	
		{
			return r==o.r && g==o.g && b==o.b;
		}	

		void aproximar_a(const Color& c)
		{
			auto cambio=[](unsigned int &mio, unsigned int suyo)
			{
				if(mio < suyo) ++mio;
				else if(mio > suyo) --mio;
			};

			cambio(r, c.r);
			cambio(g, c.g);
			cambio(b, c.b);		
		}

		friend std::ostream &operator<<(std::ostream &os, Color &c);
	};

	//////////////
	//Propiedades
	private:

	Color actual;
	Color deseado;	

	///////////////
	//Métodos internos.
	void escoger_nuevo_color();

	///////////////
	//Interface pública.
	public:
	
	Controlador_ejemplo(Director_estados& d);
	~Controlador_ejemplo();

	virtual void preloop(Input_base& input, float delta);
	virtual void postloop(Input_base& input, float delta);
	virtual void loop(Input_base& input, float delta);
	virtual void dibujar(DLibV::Pantalla& pantalla);
};

#endif
