#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Casella.h"

using namespace std;

class Quadrante
{	
	private:
		std::vector<std::vector<Casella> > griglia;
		int dimensioneCasella;
		int calcolaCoordinata(int x);
		int calcolaPosizione(int i);
		int numeroQuadrante;

	public:
		void pulisciCasella(int x, int y);
		void sporcaCasella(int x, int y);
		void occupaCasella(int x, int y);
		void liberaCasella(int x, int y);
		void creaCasella(int x, int y);
		void creaRiga(int y);
		void creaCaselleTo(int x, int y);
		bool exist(int x, int y);
		Casella getCasella(int x, int y);
		void toString();

		
	Quadrante(int _dimensioneCasella, int _quadrante)
	{
		dimensioneCasella = _dimensioneCasella;
		numeroQuadrante = _quadrante;
	}
		
	Quadrante()
	{
		
	}
	
	~Quadrante()
	{
		
	}
};
