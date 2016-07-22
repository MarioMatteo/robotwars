
#include "Quadrante.h"

int	Quadrante::calcolaCoordinata(int x)
{
	//return ((x - dimensioneCasella/2)/dimensioneCasella) + 1;
	return x/dimensioneCasella;
}

int Quadrante::calcolaPosizione(int i)
{
	return (i)*dimensioneCasella + dimensioneCasella/2;
}

bool Quadrante::exist(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	bool risultato = false;
	if(griglia.size() > i)
	{
		if(griglia.at(i).size() > j)
		{
			risultato = true;
		}
	}
	return risultato;
}


void Quadrante::pulisciCasella(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	
	if(exist(x, y))
		griglia.at(i).at(j).setSporco(false);
}

void Quadrante::sporcaCasella(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	
	if(exist(x, y))
		griglia.at(i).at(j).setSporco(true);
}

void Quadrante::occupaCasella(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
		
	if(exist(x, y))
	{
		if(!griglia.at(i).at(j).isExist())
		{
			griglia.at(i).at(j).setExist(true);
		}
		griglia.at(i).at(j).setSporco(false);
		griglia.at(i).at(j).setOstacolo(true);
	}	
}

void Quadrante::liberaCasella(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	
	if(exist(x, y))
	{
		griglia.at(i).at(j).setOstacolo(false);
		griglia.at(i).at(j).setSporco(true);
	}
}

void Quadrante::creaCasella(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	
	if(exist(x, y))
	{
		griglia.at(i).at(j).setExist(true);
		griglia.at(i).at(j).setSporco(true);
	}
}

void Quadrante::creaRiga(int y)
{
	int i = calcolaCoordinata(y);
	int length = griglia.size();
	while(i > length-1)
	{
		std::vector<Casella> nuovaRiga;
		nuovaRiga.push_back(Casella(dimensioneCasella/2, calcolaPosizione(length), dimensioneCasella, numeroQuadrante));
		griglia.push_back(nuovaRiga);
		length = griglia.size();
	}
}

void Quadrante::creaCaselleTo(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	if(griglia.size() == 0)
		creaRiga(x);
	else if(i > griglia.size()-1)
		creaRiga(x);
		
	int length = griglia.at(i).size();

	while(j > length-1)
	{
		
		griglia.at(i).push_back(Casella(dimensioneCasella/2, calcolaPosizione(length), dimensioneCasella, numeroQuadrante));
		griglia.at(i).at(length).setExist(false);
		length = griglia.at(i).size();
	}
	
	creaCasella(x,y);
}

Casella Quadrante::getCasella(int x, int y)
{
	int i = calcolaCoordinata(x);
	int j = calcolaCoordinata(y);
	griglia.at(i).at(j);
}


void Quadrante::toString()
{
	int i = 0;
	int j = 0;
	
	for(i = 0; i < griglia.size(); i++)
	{
		cout << i << " :";
		
		for(j = 0; j < griglia.at(i).size(); j++)
		{
			if(!griglia.at(i).at(j).isExist())
				cout << "?";
			else if(griglia.at(i).at(j).isOstacolo())
				cout << "O";
			else if(griglia.at(i).at(j).isSporco())
				cout << "S";
			else if(!griglia.at(i).at(j).isSporco())
				cout << "P";
		}
		cout <<endl;
	}
}

