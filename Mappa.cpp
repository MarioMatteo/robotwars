#include "Mappa.h"
	
		
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

Quadrante Mappa::getQuadrante(int i)
{
	return quadranti[i];
}

int* Mappa::pulisciCoordinate(int x, int y)
{
	int coordinate[2];
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
	return coordinate;
}

void Mappa::creaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int* coordinate = pulisciCoordinate(x,y);
	quadranti[i].creaCaselleTo(coordinate[0], coordinate[1]);
}

void Mappa::pulisciCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int* coordinate = pulisciCoordinate(x,y);
	quadranti[i].pulisciCasella(coordinate[0], coordinate[1]);
}

void Mappa::sporcaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int* coordinate = pulisciCoordinate(x,y);
	quadranti[i].sporcaCasella(coordinate[0], coordinate[1]);
}

void Mappa::occupaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int* coordinate = pulisciCoordinate(x,y);
	quadranti[i].occupaCasella(coordinate[0], coordinate[1]);
}

void Mappa::liberaCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int* coordinate = pulisciCoordinate(x,y);
	quadranti[i].liberaCasella(coordinate[0], coordinate[1]);
}

Casella Mappa::getCasella(int x, int y)
{
	int i = checkQuadrante(x,y)-1;
	int* coordinate = pulisciCoordinate(x,y);
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
