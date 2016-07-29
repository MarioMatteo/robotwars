/*
 * SensorReadingBox.cpp
 *
 *  Created on: 27/giu/2016
 *      Author: gabrielefcg
 */

#include "SensorReadingBox.h"

//A volte non vede gli ostacoli

SensorReadingBox::SensorReadingBox(Mappa* mappa, ArRobot* robot,  int unit, int num_cell) {
	this->mappa=mappa;
	this->unit=unit;
	this->num_cell=num_cell;
	this->robot=robot;
	this->laser=NULL;

}
SensorReadingBox::~SensorReadingBox() {
	// TODO Auto-generated destructor stub
}

void SensorReadingBox::readSensor()
{
	robot->lock();

	// lettura posizione robot e cambio coordinate per portarle nel sistema utilizzato dalla classe Mappa
	ArPose current_position= robot->getPose();
	int x_robot,y_robot, direction_robot;
	x_robot=current_position.getX();
	y_robot=current_position.getY();
	int my_position[2];
	MyUtil::coordinateFromRobotToMine(x_robot,y_robot,my_position);
	x_robot=my_position[0];
	y_robot=my_position[1];

	// direzione del robot in gradi rispetto alla direzione nel punto di avvio iniziale
	direction_robot=current_position.getTh();

	// x e y temporanee nel "nostro" sistema di coordinate
	int y_temp = 0;
	int x_temp = 0;
	// x e y temporanee del robot
	int y_temp_robot = 0;
	int x_temp_robot = 0;

	// inizializzazione laser per rilevamento ostacoli
	std::map<int,ArLaser*> *lasers = robot->getLaserMap(); 
	std::map<int,ArLaser*>::const_iterator i = lasers->begin();
	laser = (*i).second;
	int max_range = laser->getAbsoluteMaxRange(); // laser rileva un ostacolo a distanza max_range se non c'è un ostacolo
	
	laser->lockDevice();
	int direction_approssimated = this->approssimation(direction_robot); // discretizza direzione robot in uno degli 8 settori uniformi circostanti al robot
	ArPose* obstacle = new ArPose(); // conterrà le coordinate dell'ostacolo rilevato
	switch(direction_approssimated) // 8 possibili valori da 0 a 7 inclusi
	{
		case 0:
			for(int row = 1; row <= num_cell; row++)
			{
				//quantizzare x temp e y temp per far si che siano esattamente nel centro di una cella
				//serve per mantenere i centri delle celle al posto giusto nella "griglia"
				y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=-row;col<=row;col++)
				{
					x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);
					//double distance = robot->checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle,&laser );
					int centro_casella_robot[2]; // vettore che conterra' il risultato della trasformazione
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];

					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					mappa->creaCasella(x_temp,y_temp);
					cout<<"distanza "<<distance<<endl;
					if(distance >= 0  &&  distance < max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;
			
		case 45:
			for(int row=0;row<=num_cell;row++)
			{
				y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=0;col<=num_cell;col++)
				{

					x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);
					if(x_robot!=x_temp||y_robot!=y_temp)
					{
						int centro_casella_robot[2];
						MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
						x_temp_robot=centro_casella_robot[0];
						y_temp_robot=centro_casella_robot[1];
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						mappa->creaCasella(x_temp,y_temp);
						if(distance >=0  &&  distance < max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;
		case -45:
			for(int row=0;row<=num_cell;row++)
			{
				y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=0;col>=-num_cell;col--)
				{

					x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);
					if(x_robot!=x_temp||y_robot!=y_temp)
					{
						int centro_casella_robot[2];
						MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
						x_temp_robot=centro_casella_robot[0];
						y_temp_robot=centro_casella_robot[1];
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						mappa->creaCasella(x_temp,y_temp);
						if(distance>=0&&distance<max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;
		case 90:
			for(int col=1;col<=num_cell;col++)
			{
				x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);

				for(int row=-col;row<=col;row++)
				{
					y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
					int centro_casella_robot[2];
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					mappa->creaCasella(x_temp,y_temp);
					if(distance>=0&&distance<max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;
		case -90:
			for(int col=-1;col>=-num_cell;col--)
			{
				x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);

				for(int row=-col;row<=col;row++)
				{
					y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
					int centro_casella_robot[2];
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					mappa->creaCasella(x_temp,y_temp);
					if(distance>=0&&distance<max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;
		case 135:
			for(int row=0;row>=-num_cell;row--)
			{
				y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=0;col<=num_cell;col++)
				{

					x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);
					if(x_robot!=x_temp||y_robot!=y_temp)
					{
						int centro_casella_robot[2];
						MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
						x_temp_robot=centro_casella_robot[0];
						y_temp_robot=centro_casella_robot[1];
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						mappa->creaCasella(x_temp,y_temp);
						if(distance>=0&&distance<max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;
		case -135:
			for(int row=0;row>=-num_cell;row--)
			{
				y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=0;col>=-num_cell;col--)
				{

					x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);
					if(x_robot!=x_temp||y_robot!=y_temp)
					{
						int centro_casella_robot[2];
						MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
						x_temp_robot=centro_casella_robot[0];
						y_temp_robot=centro_casella_robot[1];
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						mappa->creaCasella(x_temp,y_temp);
						if(distance>=0&&distance<max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;
		case 180:
			for(int row=-1;row>=-num_cell;row--)
			{
				y_temp =MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=-row;col<=row;col++)
				{
					x_temp =MyUtil::centraCoordinate(x_robot + unit*col,unit);
					int centro_casella_robot[2];
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);
					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					mappa->creaCasella(x_temp,y_temp);
					if(distance>=0&&distance<max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						cout<<"casella lbera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;

		}



	laser->unlockDevice();

	robot->unlock();
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

