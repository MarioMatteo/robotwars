/*
 * PathPlanning.h
 *
 *  Created on: 22/giu/2016
 *      Author: gabrielefcg
 */

#ifndef MYPATHPLANNING_H_
#define MYPATHPLANNING_H_
#include "Mappa.h"
#include "Casella.h"
#include "Aria.h"
#include <math.h>

class MyPathPlanning {
private:
	double fromDirectionToAngle(int direzione);
	int unit;
	Mappa* mappa;
	ArRobot* robot;
	int valutazioneEuristicheUguali(int x[],int y[], int direzione1, int direzione2);
	double fromCartToPolar(int origin_x, int origin_y, int x, int y);
	int sceltaEuristica(int x[],int y[], int direzione);
	double MediaSettore(int direzione, Casella casella);
	void getXY(int* x, int* y);
	int approssimation2(double angle);
public:
	MyPathPlanning(int unit, Mappa* mappa, ArRobot* robot);

	virtual ~MyPathPlanning();
	void percorri();

};

#endif /* PATHPLANNING_H_ */
