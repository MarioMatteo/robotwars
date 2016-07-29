#ifndef MAPPA_H_
#define MAPPA_H_

#include <stdlib.h>  
#include "Quadrante.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class Mappa
{
	private:
		std::vector<Quadrante> quadranti;
		void pulisciCoordinate(int x, int y, int coordinate[]);
		
	public:
		void getTutteCaselle(std::vector<Casella>* lista);
		Mappa(int dimensioneCasella);
		~Mappa();
		int checkQuadrante(int x, int y);
		Quadrante getQuadrante(int i);	
		void creaCasella(int x, int y);
		void pulisciCasella(int x, int y);
		void sporcaCasella(int x, int y);
		void occupaCasella(int x, int y);
		void liberaCasella(int x, int y); 
		Casella* getCasella(int x, int y);
		void toString();

	
};

#endif
