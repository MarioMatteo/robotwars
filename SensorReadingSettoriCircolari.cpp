/*
 * SensorReading.cpp
 *
 *  Created on: 23/giu/2016
 *      Author: gabrielefcg
 */

#include "SensorReading.h"
#include <stdio.h>
#include <stdlib.h>
SensorReading::SensorReading(Mappa mappa, ArLaser laser,ArRobot robot,  double angle_sector, double max_angle) {
	this->mappa=mappa;
	this->laser=laser;
	this->angle_sector=angle_sector;
	this->max_angle=max_angle;
	this->lecture_sector=floor(max_angle/angle_sector);
	this->start_angle_reading=-90-(240-angle_sector*((double)this->lecture_sector))/2;
	this->robot=robot;

}

SensorReading::~SensorReading() {
	// TODO Auto-generated destructor stub
}

void SensorReading::leggiSensori()
{
	robot.lock();
	laser->lockDevice();
	for(int i=0;i<(max_angle/angle_sector);i++)
	{
		double start_angle_sector=this->start_angle_reading+((double)i)*this->angle_sector;
		double end_angle_sector=this->start_angle_reading+this->angle_sector;
		this->leggiSettore(start_angle_sector,end_angle_sector);
	}
	laser->unlockDevice();

	robot.unlock();
}


/*
void SensorReading::detectNewCell(double start_angle, double end_angle)
{
//aggiungo solo le caselle lungo i bordi che quelle in mezzo vengono create in automatico
	ArPose current_position= robot.getPose();
	ArPose origine= new ArPose();
	//devo conoscere l'angolo rispetto al quale il robot è messo con l'origine
	double angolo_robot_origine=robot->findAngleTo(origine);

	int x_robot,y_robot;
	x_robot=current_position.getX();
	y_robot=current_position.getY();
	for(int i=1;i<=(max_dist/unit);i++)
	{
		//creo le caselle posizionate ai bordi esterni del settore conoscendo l'angolo con cui sono messe rispetto al robot e l'angolo con cui il robot è messo rispetto
		//all'orgine
		int x=sin(start_angle)*i*unit+x_robot*sin(angolo_robot_origine);
		int y=cos(start_angle)*i*unit+y_robot*cos(angolo_robot_origine);

		mappa->aggiungiCasella(x,y);
		int x=sin(end_angle)*i*unit+x_robot*sin(angolo_robot_origine);
		int y=cos(end_angle)*i*unit+y_robot*cos(angolo_robot_origine);

		mappa->aggiungiCasella(x,y);
	}

	for(int i=1;i<=(sin(end_angle-start_angle)*max_dist)/unit;i++)
	{
		//creo le caselle posizionate sul limite
		int x=sin(start_angle)*max_dist+i*unit+x_robot*sin(angolo_robot_origine);
		int y=cos(i*end_angle-start_angle/(sin(end_angle-start_angle)*max_dist)/unit)*max_dist+y_robot*cos(angolo_robot_origine);
		mappa->aggiungiCasella(x,y);
	}

}

void SensorReading::detectObstacle(double start_angle, double end_angle)
{
	//double max_distance=lase->getMax

	// There is a utility to find the closest reading wthin a range of degrees around the laser, here we use this laser's full field of view (start to end)
	// If there are no valid closest readings within the given range, dist will be greater than laser->getMaxRange().
	double angle = 0;
	ArPose origine=new ArPose();
	double angolo_robot_origine=robot->findAngleTo(origine);
	double dist_obstacle = laser->currentReadingPolar(start_angle, end_angle, &angle);
	ArPose current_position= robot.getPose();
	int x_robot,y_robot;
	x_robot=current_position.getX();
	y_robot=current_position.getY();

	if(dist_obstacle<max_dist)
						 {

	for(int row=0;row<mappa->numRow();row++)
	{
		int numCol=mappa->getRow(row).size();
		for(int col=0;col<numCol;col++)//spazzolo tutta la mappa salvando le due medie
		{
			Casella casella=mappa.getCasella(row,col);
			int x_casella=casella.getX();
			int y_casella=casella.getY();
			double angolo_casella=this->fromCartToPolar(x_robot,y_robot,x_casella,y_casella);

			//if(start_angle<=angolo_casella&&end_angle>=angolo_casella)
			{
				 double distanza_casella=this->distance(x_robot,y_robot,x_casella,y_casella);
				 if(distanza_casella<max_dist)
				 {


						 if(distanza_casella<dist_obstacle)
						 {
							 casella.setSporca(); ///è una normale casella forse come da default?
						 }
						 else
						 {
							 //visto che segna il primo ostacolo segno come ostacolo tutte le caselle messe dopo
							 int x_ostacolo=cos(angle+angolor_robot_origine)*(dist_obstacle+x_robot);
							 if(x_casella>=x_ostacolo)
							 {
								 casella.setObstacle();
							 }
							 else
							 {
								 casella.setSporca();
							 }

						 }
					 }
				 }

			}
		}
		}
	}

*/


}

double SensorReading::fromCartToPolar(int origin_x, int origin_y, int x, int y)
  {
	  return atan((double)(origin_x+x)/(double)(origin_y+y));
  }
double 	SensorReading::distance(int x1,int y1, int x2,int y2)
{
	return sqrt( abs((double)((x1-x2)*(x1-x2)-(y1-y2)*(y1-y2))));
}
