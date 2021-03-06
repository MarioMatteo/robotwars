/*
 * SensorReadingBox.h
 *
 *  Created on: 27/giu/2016
 *      Author: gabrielefcg
 */

#ifndef SENSORREADINGBOX_H_
#define SENSORREADINGBOX_H_

#include "Aria.h"
#include "Mappa.h"
//#include "Arnl.h"
#include "MyUtil.h"

class SensorReadingBox {
private:
	int num_cell;
	int unit;
	Mappa* mappa;
	ArLaser* laser;
	ArRobot* robot;
	void readCell(int x, int y);
	int approssimation(double angle);
	void scan_for_obstacles(vector<int>& rows_ind, vector<int>& cols_ind, int xy_robot[2], ArLaser* laser, Mappa* mappa);
public:
	void readSensor();
	SensorReadingBox(Mappa* mappa, ArRobot* robot,  int unit, int num_cell);
	virtual ~SensorReadingBox();
};

#endif /* SENSORREADINGBOX_H_ */
