/*
 * SensorReadingBox.cpp
 *
 *  Created on: 27/giu/2016
 *      Author: gabrielefcg
 */

#include "SensorReadingBox.h"

SensorReadingBox::SensorReadingBox(Mappa mappa, ArLaser laser,ArRobot robot,  int unit, int num_cell) {
	this->mappa=mappa;
	this->laser=laser;
	this->unit=unit;
	this->num_cell=num_cell;
	this->robot=robot;

}
SensorReadingBox::~SensorReadingBox() {
	// TODO Auto-generated destructor stub
}

void SensorReadingBox::readSensor()
{
	robot.lock();
	laser->lockDevice();

	ArPose current_position= robot.getPose();
	ArPose obstacle = new ArPose();
	int x_robot,y_robot, direction_robot;
	x_robot=current_position.getX();
	y_robot=current_position.getY();
	direction_robot=current_position.getTh();

	int y_temp = 0;
	int x_temp = 0;


		int direction_approssimated=this->approssimation(direction_robot);
		switch(direction_approssimated)
		{
			case 0:
				for(int row=1;row<=num_cell;row++)
				{
					y_temp = y_robot + unit*row;
					for(int col=-row;col<=row;col++)
					{
						x_temp = x_robot + unit*col;
						double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
						mappa->creaCasella(x_temp,y_robot);
						if(distance>=0)
						{
							mappa->occupaCasella(x_temp,y_temp);
						}
					}
				}
				break;
			case 45:
				for(int row=0;row<=num_cell;row++)
				{
					y_temp = y_robot + unit*row;
					for(int col=0;col<=num_cell;col++)
					{

						x_temp = x_robot + unit*col;
						if(x_robot!=x_temp||y_robot!=y_temp)
						{
							double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
							mappa->creaCasella(x_temp,y_robot);
							if(distance>=0)
							{
								mappa->occupaCasella(x_temp,y_temp);
							}
						}
					}
				}
				break;
			case -45:
				for(int row=0;row>=-num_cell;row--)
				{
					y_temp = y_robot + unit*row;
					for(int col=0;col>=-num_cell;col--)
					{

						x_temp = x_robot + unit*col;
						if(x_robot!=x_temp||y_robot!=y_temp)
						{
							double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
							mappa->creaCasella(x_temp,y_robot);
							if(distance>=0)
							{
								mappa->occupaCasella(x_temp,y_temp);
							}
						}
					}
				}
				break;
			case 90:
				for(int col=1;col<=num_cell;col++)
				{
					x_temp = x_robot + unit*col;

					for(int row=-col;row<=col;row++)
					{
						y_temp = y_robot + unit*row;
						double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
						mappa->creaCasella(x_temp,y_robot);
						if(distance>=0)
						{
							mappa->occupaCasella(x_temp,y_temp);
						}
					}
				}
				break;
			case -90:
				for(int col=-1;col>=-num_cell;col--)
				{
					x_temp = x_robot + unit*col;

					for(int row=-col;row<=col;row++)
					{
						y_temp = y_robot + unit*row;
						double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
						mappa->creaCasella(x_temp,y_robot);
						if(distance>=0)
						{
							mappa->occupaCasella(x_temp,y_temp);
						}
					}
				}
				break;
			case 135:
				for(int row=0;row>=-num_cell;row--)
				{
					y_temp = y_robot + unit*row;
					for(int col=0;col<=num_cell;col++)
					{

						x_temp = x_robot + unit*col;
						if(x_robot!=x_temp||y_robot!=y_temp)
						{
							double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
							mappa->creaCasella(x_temp,y_robot);
							if(distance>=0)
							{
								mappa->occupaCasella(x_temp,y_temp);
							}
						}
					}
				}
				break;
			case -135:
				for(int row=0;row>=-num_cell;row--)
				{
					y_temp = y_robot + unit*row;
					for(int col=0;col<=num_cell;col++)
					{

						x_temp = x_robot + unit*col;
						if(x_robot!=x_temp||y_robot!=y_temp)
						{
							double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
							mappa->creaCasella(x_temp,y_robot);
							if(distance>=0)
							{
								mappa->occupaCasella(x_temp,y_temp);
							}
						}
					}
				}
				break;
			case 180:
				for(int row=-1;row>=-num_cell;row--)
				{
					y_temp = y_robot + unit*row;
					for(int col=-row;col<=row;col++)
					{
						x_temp = x_robot + unit*col;
						double distance = robot.checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle, laser);
						mappa->creaCasella(x_temp,y_robot);
						if(distance>=0)
						{
							mappa->occupaCasella(x_temp,y_temp);
						}
					}
				}
				break;

		}



	laser->unlockDevice();

	robot.unlock();
}
//Approssima l'angolo di orientamento del robot con gli angoli delle nostre 8 possibili direzioni
int SensorReadingBox::approssimation(double angle)
{
	if(angle<=(0+22.5)&&angle>=(0-22.5))
	{
		return 0;
	}
	if(angle<=(90+22.5)&&angle>=(90-22.5))
	{
		return 90;
	}
	if(angle<=(-90+22.5)&&angle>=(-90-22.5))
	{
		return -90;
	}
	if(angle<=(45+22.5)&&angle>=(45-22.5))
	{
		return 45;
	}

	if(angle<=(-45+22.5)&&angle>=(-45-22.5))
	{
		return -45;
	}
	if(angle<=(-135+22.5)&&angle>=(-135-22.5))
	{
		return -135;
	}
	if(angle<=(135+22.5)&&angle>=(135-22.5))
	{
		return 135;
	}
	return 180;
}
