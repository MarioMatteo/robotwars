/*
 * SensorReadingBox.cpp
 *
 *  Created on: 27/giu/2016
 *      Author: gabrielefcg
 */

#include "SensorReadingBox.h"
#include <math.h>
#include <limits>
#include <vector>

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

//Altrimenti il robot leggeva anche la casella su cui era posizionato quando faceva letture in diagonale
	x_robot=MyUtil::centraCoordinate(my_position[0],unit);
	y_robot=MyUtil::centraCoordinate(my_position[1],unit);
	int xy_robot[] = {x_robot, y_robot};	
	
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
	//cout<<lasers->size()<<endl;
	std::map<int,ArLaser*>::const_iterator i = lasers->begin();
	laser = (*i).second;

	//int max_range = laser->getAbsoluteMaxRange(); // laser rileva un ostacolo a distanza max_range se non c'è un ostacolo
	int max_range = laser->getMaxRange();
	//cout << "Laser max range: " << max_range << endl;
	laser->lockDevice();
	int direction_approssimated = this->approssimation(direction_robot); // discretizza direzione robot in uno degli 8 settori uniformi circostanti al robot
	ArPose* obstacle = new ArPose(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()); // conterrà le coordinate dell'ostacolo rilevato
	
	cout << "- STATO CASELLE FRONTALI" << endl;
	cout << "Posizione attuale: x = " << x_robot << " y = " << y_robot << endl;
	direction_approssimated *= -1;
	cout << "Direzione robot: " << direction_approssimated << endl << endl;
	switch(direction_approssimated) // 8 possibili valori da -180 a 135 ad intervalli di 45
	{
		case 0:
		{
			int rows_scan[] = {1};
			int cols_scan[] = {-1,0,1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
			/*for(int row = 1; row <= num_cell; row++)
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

					//x_temp_robot=centro_casella_robot[0]-x_robot;
					//y_temp_robot=centro_casella_robot[1]-y_robot;
					
					obstacle = new ArPose();
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					cout << "coordinate ostacolo dopo reading: x = " << obstacle->getX() << " y = " << obstacle->getY() << endl;
					//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					if(!isnan(obstacle->getX()))
					{
						int coord_obstacle_on_map[2];
						MyUtil::coordinateFromRobotToMine(obstacle->getX(), obstacle->getY(), coord_obstacle_on_map);
						distance = sqrt(pow(coord_obstacle_on_map[0] - x_robot, 2) + pow(coord_obstacle_on_map[1] - y_robot, 2));
					}
					mappa->creaCasella(x_temp,y_temp);
					cout<<"distanza ostacolo: "<< distance <<endl;
					if(obstacle != NULL)
					{
						*//*int coord_obstacle_on_map[2];
						MyUtil::coordinateFromRobotToMine(obstacle->getX(), obstacle->getY(), coord_obstacle_on_map);
						cout << "Ostacolo in : x = " << coord_obstacle_on_map[0] << " y = " << coord_obstacle_on_map[1] << endl; *//*
					}
					//if(distance > 0  &&  distance < max_range)
					if(!isnan(obstacle->getX()))
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}

						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;*/
		}
			
		case 45:
		{
			int rows_scan[] = {0,1};
			int cols_scan[] = {0,1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int row=0;row<=num_cell;row++)
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

						//x_temp_robot=centro_casella_robot[0]-x_robot;
						//y_temp_robot=centro_casella_robot[1]-y_robot;
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle ); 

						mappa->creaCasella(x_temp,y_temp);
						if(distance >0  &&  distance < max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;*/
		case -45:
		{
			int rows_scan[] = {0,1};
			int cols_scan[] = {0,-1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int row=0;row<=num_cell;row++)
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

						//x_temp_robot=centro_casella_robot[0]-x_robot;
						//y_temp_robot=centro_casella_robot[1]-y_robot;
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );



						mappa->creaCasella(x_temp,y_temp);
						if(distance>0&&distance<max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;*/
		case 90:
		{
			int rows_scan[] = {-1,0,1};
			int cols_scan[] = {1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int col=1;col<=num_cell;col++)
			{
				x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);

				for(int row=-col;row<=col;row++)
				{
					y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
					int centro_casella_robot[2];
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);

					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];

					//x_temp_robot=centro_casella_robot[0]-x_robot;
					//y_temp_robot=centro_casella_robot[1]-y_robot;
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );



					mappa->creaCasella(x_temp,y_temp);
					if(distance>0&&distance<max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;*/
		case -90:
		{
			int rows_scan[] = {-1,0,1};
			int cols_scan[] = {-1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int col=-1;col>=-num_cell;col--)
			{
				x_temp = MyUtil::centraCoordinate(x_robot + unit*col,unit);

				for(int row=col;row<=-col;row++)
				{
					y_temp = MyUtil::centraCoordinate(y_robot + unit*row,unit);
					int centro_casella_robot[2];
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);

					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];

					//x_temp_robot=centro_casella_robot[0]-x_robot;
					//y_temp_robot=centro_casella_robot[1]-y_robot;
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );



					mappa->creaCasella(x_temp,y_temp);
					if(distance>=0&&distance<max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;*/
		case 135:
		{
			int rows_scan[] = {0,-1};
			int cols_scan[] = {0,1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int row=0;row>=-num_cell;row--)
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

						//x_temp_robot=centro_casella_robot[0]-x_robot;
						//y_temp_robot=centro_casella_robot[1]-y_robot;
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );



						mappa->creaCasella(x_temp,y_temp);
						if(distance>=0&&distance<max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;*/
		case -135:
		{
			int rows_scan[] = {0,-1};
			int cols_scan[] = {0,-1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int row=0;row>=-num_cell;row--)
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

						//x_temp_robot=centro_casella_robot[0]-x_robot;
						//y_temp_robot=centro_casella_robot[1]-y_robot;
						double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
						//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );



						mappa->creaCasella(x_temp,y_temp);
						if(distance>=0&&distance<max_range)
						{
							mappa->occupaCasella(x_temp,y_temp);
							cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
						}
						else
						{
							//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
						if(mappa->getCasella(x_temp,y_temp)->isOstacolo()==true)
						{
							mappa->liberaCasella(x_temp,y_temp);
						}
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
						}
					}
				}
			}
			break;*/
		case 180:
		{
			int rows_scan[] = {-1};
			int cols_scan[] = {-1,0,1};
			std::vector<int> rows_ind (rows_scan, rows_scan + sizeof(rows_scan) / sizeof(int));
			std::vector<int> cols_ind (cols_scan, cols_scan + sizeof(cols_scan) / sizeof(int));
			scan_for_obstacles(rows_ind, cols_ind, xy_robot, laser, mappa);
			break;
		}
			/*for(int row=-1;row>=-num_cell;row--)
			{
				y_temp =MyUtil::centraCoordinate(y_robot + unit*row,unit);
				for(int col=row;col<=-row;col++)
				{
					x_temp =MyUtil::centraCoordinate(x_robot + unit*col,unit);
					int centro_casella_robot[2];
					MyUtil::coordinateFromMineToRobot(x_temp,y_temp, centro_casella_robot);

					x_temp_robot=centro_casella_robot[0];
					y_temp_robot=centro_casella_robot[1];

					//x_temp_robot=centro_casella_robot[0]-x_robot;
					//y_temp_robot=centro_casella_robot[1]-y_robot;
					double distance = laser->currentReadingBox((double) x_temp_robot-unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
					//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );



					mappa->creaCasella(x_temp,y_temp);
					if(distance>0&&distance<max_range)
					{
						mappa->occupaCasella(x_temp,y_temp);
						cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
					}
					else
					{
						cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
					}
				}
			}
			break;*/

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

void SensorReadingBox::scan_for_obstacles(vector<int>& rows_ind, vector<int>& cols_ind, int xy_robot[2], ArLaser* laser, Mappa* mappa)
{
	int max_range = laser->getMaxRange();
	ArPose* obstacle;
	for(int row = 0; row < rows_ind.size(); row++)
	{
		//quantizzare x temp e y temp per far si che siano esattamente nel centro di una cella
		//serve per mantenere i centri delle celle al posto giusto nella "griglia"
		int y_temp = MyUtil::centraCoordinate(xy_robot[1] + unit * rows_ind.at(row), unit);
		int y_temp_RB = MyUtil::centraCoordinate(unit * rows_ind.at(row), unit);
		for(int col = 0; col < cols_ind.size(); col++)
		{
			int x_temp = MyUtil::centraCoordinate(xy_robot[0] + unit * cols_ind.at(col), unit);
			int x_temp_RB = MyUtil::centraCoordinate(unit * cols_ind.at(col), unit);
			if(xy_robot[0] != x_temp  ||  xy_robot[1] != y_temp)
			{			
				//cout << "coord originali " << unit*cols_ind.at(col) << " :: coord centrate: " << x_temp_RB << endl; 
				//double distance = robot->checkRangeDevicesCurrentBox((double) x_temp-unit/2,(double) y_temp-unit/2, (double) x_temp+unit/2, (double) y_temp+unit/2, obstacle,&laser );
				int centro_casella_robot[2]; // vettore che conterra' il risultato della trasformazione
				MyUtil::coordinateFromMineToRobot(x_temp_RB, y_temp_RB, centro_casella_robot);
				int x_temp_robot=centro_casella_robot[0];
				int y_temp_robot=centro_casella_robot[1];

				//x_temp_robot=centro_casella_robot[0]-x_robot;
				//y_temp_robot=centro_casella_robot[1]-y_robot;
			
				obstacle = new ArPose();
				double distance = laser->currentReadingBox((double) x_temp_robot-unit/2, (double) y_temp_robot-unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot+unit/2, obstacle);
				cout << "coordinate ostacolo dopo reading: x = " << obstacle->getX() << " y = " << obstacle->getY() << endl;
				//double distance = laser->currentReadingBox((double) x_temp_robot+unit/2,(double) y_temp_robot+unit/2, (double) x_temp_robot+unit/2, (double) y_temp_robot-unit/2, obstacle );
			
				/*if(obstacle->getX() == 0 && obstacle->getY() == 0 && distance >= 20000)
				{
					int coord_obstacle_on_map[2];
					MyUtil::coordinateFromRobotToMine(obstacle->getX(), obstacle->getY(), coord_obstacle_on_map);
					distance = sqrt(pow(coord_obstacle_on_map[0] - x_robot, 2) + pow(coord_obstacle_on_map[1] - y_robot, 2));
				}*/
				mappa->creaCasella(x_temp, y_temp);
				//cout<<"distanza ostacolo: "<< distance <<endl;
				/*if(obstacle != NULL)
				{
					int coord_obstacle_on_map[2];
					MyUtil::coordinateFromRobotToMine(obstacle->getX(), obstacle->getY(), coord_obstacle_on_map);
					cout << "Ostacolo in : x = " << coord_obstacle_on_map[0] << " y = " << coord_obstacle_on_map[1] << endl; 
				}*/
				if(distance > 0  &&  distance < max_range)
				{
					mappa->occupaCasella(x_temp,y_temp);
					cout<<"casella occupata "<<x_temp<<" , "<<y_temp<<endl;
				}
				else
				{
					//Se la casella prima era vista come un ostacolo e ora no allora la considero libera
					if(mappa->getCasella(x_temp,y_temp)->isOstacolo() == true)
					{
						mappa->liberaCasella(x_temp,y_temp);
					}

					cout<<"casella libera  "<<x_temp<<" , "<<y_temp<<endl;
				}
			}
			else
			{
				cout << "sto controllando la stessa casella del robot" << endl;
			}
		}
	}
}
