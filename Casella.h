#ifndef CASELLA_H_
#define CASELLA_H_
#include <stdlib.h>  

class Casella{
	private:
		bool ostacolo;
		bool sporco;
		bool exist;
		int x;
		int y;
		int dimensione;
		int quadrante;

	public:
		Casella(int _x, int _y, int _dimensione, int _quadrante);
		virtual ~Casella();
		//GETTER
		bool isOstacolo();
		bool isSporco();
		int getX();
		int getY();
		int getQuadrante();
		bool isExist();
		//SETTER
		void setOstacolo(bool _ostacolo);
		void setSporco(bool _sporco);
		void setExist(bool _exist);
		//METODI VARI
		bool isInside(int _x, int _y);
		



};
#endif
