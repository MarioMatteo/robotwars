#include "Mappa.h"

Mappa::Mappa(int dimensioneCasella)
{

	int i = 0;
	for(i = 0; i < 4; i++)
	{	Quadrante quadrante = Quadrante(dimensioneCasella, i);
		quadranti.push_back( quadrante);
	}
}
Mappa::~Mappa()
{

}
int Mappa::checkQuadrante(int x, int y)
{
	int quadrante = 0;
			
	if(x >=0 && y >= 0)
		quadrante = 1;
	else if(x < 0 && y >= 0)
		quadrante = 2;
	else if(x < 0 && y < 0)
		quadrante = 3;
	else if(x >=0 && y < 0)
		quadrante = 4;
	
	return quadrante;
}

void Mappa::getTutteCaselle(std::vector<Casella>* lista)
{
	for(int i = 0; i < 4; i++)
	{
		quadranti[i].getTutteCaselleQuadrante( lista);
	}
}


Quadrante Mappa::getQuadrante(int i)
{
	return quadranti[i];
}

void Mappa::pulisciCoordinate(int x, int y, int coordinate[])
{

	int i = checkQuadrante(x,y);
	if(i == 1 || i == 3)
	{
		coordinate[0] = abs(y);
		coordinate[1] = abs(x);
	}
	else if(i == 2 || i == 4)
	{
		coordinate[0] = abs(x);
		coordinate[1] = abs(y);
	}

}

void Mappa::creaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int coordinate[2];
	quadranti[i].creaCaselleTo(coordinate[0], coordinate[1]);
	pulisciCoordinate(x,y, coordinate);

	//cout<<"Creata casella "<<x<<" , "<<y<<endl;
}

void Mappa::pulisciCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int coordinate[2];
		pulisciCoordinate(x,y, coordinate);
	quadranti[i].pulisciCasella(coordinate[0], coordinate[1]);
}

void Mappa::sporcaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int coordinate[2];
		pulisciCoordinate(x,y, coordinate);
	quadranti[i].sporcaCasella(coordinate[0], coordinate[1]);
}

void Mappa::occupaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int coordinate[2];
		pulisciCoordinate(x,y, coordinate);
	quadranti[i].occupaCasella(coordinate[0], coordinate[1]);
}

void Mappa::liberaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int coordinate[2];
	pulisciCoordinate(x,y, coordinate);
	quadranti[i].liberaCasella(coordinate[0], coordinate[1]);
}

Casella* Mappa::getCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int coordinate[2];

	pulisciCoordinate(x,y, coordinate);

	return quadranti[i].getCasella(coordinate[0], coordinate[1]);
}

void Mappa::toString()
{
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		cout << i+1 << " Quadrante" << endl;
		quadranti[i].toString();
	}
}
