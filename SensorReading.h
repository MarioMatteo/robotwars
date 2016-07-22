/*
 * SensorReading.h
 *
 *  Created on: 23/giu/2016
 *      Author: gabrielefcg
 */

#ifndef SENSORREADING_H_
#define SENSORREADING_H_

class SensorReading {
private:
	int unit;
	Mappa mappa;
	ArLaser laser;
	ArRobot robot;
	int lecture_sector;
	double max_angle;
	double angle_sector;
	double start_angle_reading;
	double max_dist;
	void detectNewCell(double start_angle, double end_angle);
	void detectObstacle(double start_angle, double end_angle);
	double fromCartToPolar(int origin_x, int origin_y, int x, int y);
	double distance(int x1,int y1, int x2,int y2);
public:
	void leggiSensori();
	SensorReading(Mappa mappa, ArLaser laser,ArRobot robot,  double angle_sector, double max_angle, double max_range, int unit);
	virtual ~SensorReading();
};

#endif /* SENSORREADING_H_ */
