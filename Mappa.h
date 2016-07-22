#include <stdlib.h>  

#include "Quadrante.h"

class Mappa
{
	private:
		Quadrante quadranti [4];
		int* pulisciCoordinate(int x, int y);
		
	public:
		int checkQuadrante(int x, int y);
		Quadrante getQuadrante(int i);	
		void creaCasella(int x, int y);
		void pulisciCasella(int x, int y);
		void sporcaCasella(int x, int y);
		void occupaCasella(int x, int y);
		void liberaCasella(int x, int y); 
		Casella getCasella(int x, int y);
		void toString();
	
	
	Mappa(int dimensioneCasella)
	{
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			quadranti[i] = Quadrante(dimensioneCasella, i);
		}
	}
	
	~Mappa()
	{
		
	}
};
