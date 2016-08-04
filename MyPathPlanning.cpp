/*
 * MyPathPlanning.cpp
 *
 *  Created on: 22/giu/2016
 *      Author: gabrielefcg
 */
#include "MyPathPlanning.h"


MyPathPlanning::MyPathPlanning(int unit, Mappa* mappa, ArRobot* robot) {
	// TODO Auto-generated constructor stub
	this->unit=unit;
	this->mappa=mappa;
	this->robot=robot;
	this->num_cell = 1;
	this->duration = 3000;


}

MyPathPlanning::~MyPathPlanning() {
	// TODO Auto-generated destructor stub
}




void MyPathPlanning::percorri()
{
	SensorReadingBox* sensor = new SensorReadingBox(mappa, robot, unit, num_cell);
	int duration = 90000;

	// Collision avoidance actions at higher priority
	ArActionStallRecover recover("stall recover",unit/11);
	ArActionBumpers bumpers;
	ArActionLimiterForwards limiterAction("speed limiter far", unit/100, unit, 110);
	//ArActionLimiterForwards limiterAction("speed limiter far", 200, 1000, 200, 1);
	
	ArActionAvoidFront avoidFrontNear("Avoid Front Near", unit/3, 110, 45);
	//ArActionAvoidFront avoidFrontNear("Avoid Front Near", 450, 200, 15);
	
	robot->addAction(&recover, 100);
	robot->addAction(&bumpers, 75);
	robot->addAction(&avoidFrontNear, 55);
	//robot->addAction(&avoidFrontNear, 90);
	
	robot->addAction(&limiterAction, 90);
	//robot->addAction(&limiterAction, 55);

	// Action to set goal to robot
	ArActionGoto gotoPoseAction("goto");
	robot->addAction(&gotoPoseAction, 50);

	// Stop action at lower priority, so the robot stops if it has no goal
	ArActionStop stopAction("stop");
	ArActionStop stopToMapping("stop_mapping");
	robot->addAction(&stopAction, 40);

	// turn on the motors, turn off amigobot sounds
	robot->enableMotors();
	robot->comInt(ArCommands::SOUNDTOG, 0);

	//int goalNum = 0;
	// fase di inizializzazione parametri posizione robot
	int direction=0;
	ArTime start;
	start.setToNow();
	int previus_x=-1;
	int previus_y=-1;
	mappa->creaCasella(0,0);

	int numero_iterazione_ciclo = 1;
	
	while (Aria::getRunning())
	{
		robot->remAction(&avoidFrontNear);	
		robot->remAction(&limiterAction);
		robot->remAction(&gotoPoseAction);
		
		cout<<"\n### INIZIO CICLO #"<< numero_iterazione_ciclo++ <<" ###"<<endl;
		robot->lock();
		int current_x;
		int current_y;

		//robot->addAction(&stopToMapping, 99);
		//cout << "stoppato" << endl;
		//ArUtil::sleep(3000);
		this->getXY(&current_x,&current_y);
		cout << "appena assegnate: x = " << current_x << " y = " << current_y << endl;
		int my_position[2];
		MyUtil::coordinateFromRobotToMine(current_x, current_y, my_position); 
		current_x = MyUtil::centraCoordinate(my_position[0], unit);
		current_y = MyUtil::centraCoordinate(my_position[1], unit);
		cout << "dopo trasformazione: x = " << current_x << " y = " << current_y << endl;

		sensor->readSensor();
		/*int my_position[2];
		MyUtil::coordinateFromRobotToMine(current_x, current_y, my_position);
		current_x = my_position[0];
		current_y = my_position[1];*/ 
		mappa->pulisciCasella(current_x, current_y);

		if(current_x != previus_x  ||  current_y != previus_y)
		{
			start.setToNow();
		}
		previus_y = current_y;
		previus_x = current_x;


		int future_x;
		int future_y;
//numerazione stile quadranti orologio, la posizione delle celle locali non cambia in base alla direzione ma in base alla direzione della mappa;
    		int x[8];
    		int y[8];

		for(int i = 0; i < 8; i++)
		{
			x[i] = current_x;
			y[i] = current_y;
			if(i >= 1  &&  i <= 3)
			{
				x[i] = x[i] + unit; //per andare dall'indirizzo della cella dove sono a quelle subito vicine
			}
			if(i >= 5  &&  i <= 7)
			{
				x[i] = x[i] - unit;
			}
			if(i == 7  ||  (i >= 0  &&  i <= 1))
			{
				y[i] = y[i] + unit;
			}
			if(i >= 3  &&  i <= 5)
			{
				y[i] = y[i] - unit;
			}
		}
		ArPose current_position = robot->getPose();
		direction = this->approssimation2(current_position.getTh()); // modificato segno current position getTh
		cout<<"direzione del robot non approssimata: "<<current_position.getTh()<<endl;
		cout<<"direzione del robot: "<<direction<<endl;
		//cout<<"prima di scelta euristica"<<endl;
		int futura_direzione = this->sceltaEuristica(x,y,direction);
		// cout<<"prima di scelta euristica"<<endl;

		if(futura_direzione == -1)
		{
			gotoPoseAction.cancelGoal();
			robot->unlock();
			ArUtil::sleep(3000);
			cout << "pulizia completata" << endl;
			break;
		}

		future_x=x[futura_direzione];
		future_y=y[futura_direzione];

		direction=futura_direzione;

		Casella* casella= mappa->getCasella(future_x,future_y);
		cout << endl;
		cout << " - DETERMINAZIONE FUTURA DIREZIONE" << endl;
		cout << "Posizione attuale: x = " << current_x << ", y = " << current_y << endl;
		//ArLog::log(ArLog::Normal, "Going to  x = %d y = %d", future_x, future_y);
		cout << "Diretto a: x = " << future_x << ", y = " << future_y << endl;
		//cout<<"nuova casella"<<endl;
		if(casella->isOstacolo())
			cout << "Casella occupata e ";
		else
			cout << "Casella libera e ";
		if(casella->isSporco())		
			cout << "sporca" << endl;
		else
			cout << "pulita" << endl;

		
		int future_goal_coordinate[2];
		MyUtil::coordinateFromMineToRobot(future_x, future_y, future_goal_coordinate);
		//robot->remAction(&stopToMapping);
		gotoPoseAction.setGoal(ArPose(future_goal_coordinate[0], future_goal_coordinate[1]));

		//int my_goal[2];
		//MyUtil::coordinateFromRobotToMine(gotoPoseAction.getGoal().getX(), gotoPoseAction.getGoal().getY(), my_goal);
		//ArLog::log(ArLog::Normal, "Going to next goal  at %d %d", my_goal[0], my_goal[1]);


		if(start.mSecSince() >= duration)
		{
			ArLog::log(ArLog::Normal, "%d seconds have elapsed. Cancelling current goal, waiting 3 seconds, and exiting.", duration/1000);
			gotoPoseAction.cancelGoal();
			robot->unlock();
			ArUtil::sleep(500);
			break;
		}

		robot->addAction(&avoidFrontNear, 55);	
		robot->addAction(&limiterAction, 90);
		robot->addAction(&gotoPoseAction, 50);
		
		robot->unlock();
		
		cout << endl;
		cout << "- STATO MAPPA" << endl;
		this->mappa->toString();
		cout << endl;
		cout<<"### FINE CICLO ###"<<endl<<endl<<endl;
		ArUtil::sleep(300);
}
cout<<"uscita dal ciclo"<<endl;

// Robot disconnected or time elapsed, shut down
Aria::exit(0);

}

//le caselle aggiornano stato occupato o meno quando ci ripasso
int MyPathPlanning::sceltaEuristica(int x[],int y[], int direzione)
{
	bool all_cleaned=true;

	int euristica_max=0;
	int indice_casella_max=0;
	int direzione_max=direzione;
	bool casella_non_visitata=false;
				
int euristiche_uguali=-1;
	//la direzione migliore è quella di andare avanti seguendo la propria direzione che all'inizio corrisponde alla direzione 0
	for(int i=0;i<8;i++)
	{
		int euristica=0;
		int direzione_analizzata=(direzione+i)%8;
		// cout<<"casella da valutare "<<x[i]<<" , "<<y[i]<<endl;
		//cout<<"direzione, direzione analizzata "<<direzione<<" , "<<direzione_analizzata<<endl;
		Casella* casella=mappa->getCasella(x[direzione_analizzata],y[direzione_analizzata]);
		//cout<<"ho ricevuto la casella"<<endl;

		if(!casella->isOstacolo() && casella->isExist()) // se è libera ed esiste
		{
      			euristica=euristica+12;
		}
		if(!casella->isExist()) // la casella esiste nella struttura dati ma non l'abbiamo ancora letta
	 	{
			euristica=euristica+1;
			casella_non_visitata=true; // usato per la terminazione della pulizia
	 	}
		if(casella->isSporco() && casella->isExist())
		{
			if(direzione_analizzata == direzione)
			{
				euristica = euristica + 6; 
			}
			if(direzione_analizzata == (direzione+1)%8  ||  direzione_analizzata == (direzione+7)%8)
			{
				euristica=euristica+5;
			}
			if(direzione_analizzata==(direzione+2)%8||direzione_analizzata==(direzione+6)%8)
			{
				euristica=euristica+4;
			}
			if(direzione_analizzata==(direzione+3)%8||direzione_analizzata==(direzione+5)%8)
			{
				euristica=euristica+3;
			}
			if(direzione_analizzata==(direzione+4)%8)
			{
				euristica=euristica+2;
			}
    		}

		if(euristica>euristica_max)
		{
			euristica_max=euristica;
			direzione_max=direzione_analizzata;
		}
		else
		{
			if(euristica==euristica_max)
			{
				direzione_max=this->valutazioneEuristicheUguali(x, y, direzione_analizzata, direzione_max);
				//cout << "direzione migliore " << direzione_max << endl;
euristiche_uguali=direzione_max;

			}
		}
		//cout<<"euristica "<<euristica<<endl;
		//cout<<"direzione "<<direzione_analizzata<<endl;
		if(direzione_analizzata==direzione_max)
		{
			indice_casella_max=i;
		}
	}
	if(euristiche_uguali==direzione_max)
{
cout<<"euristica scelta tra uguali: "<<euristiche_uguali<<endl;
}
	//cout << "euristica max" << euristica_max << endl;
	//cout << "direzione migliore " << direzione_max << endl;
	std::vector<Casella>  lista;
	mappa->getTutteCaselle(&lista);
	for(int i=0;i<(int)lista.size();i++)
	{

		Casella da_controllare=lista[i];
		//Per essere una casella sporca deve essere sporca e anche esistere, per evitare che il robot cerchi di pulire anche caselle che non esistono realmente
		if(da_controllare.isSporco() == true  &&  da_controllare.isExist() == true)
		{
			all_cleaned=false;
		}
	}
	//Se ho pulito tutte le caselle della mappa corrente e nell'intorno del robot non esistono caselle che non sono
	//state viste in precedenza allora ho finito
	//il controllo sulle caselle è stato aggiunto per evitare che il robot finisca subito se inizia rivolto verso il muro
	if(all_cleaned == true  &&  casella_non_visitata == false)
	{
		return -1; // significa che ha pulito tutto
	}
	return direzione_max;
}

int MyPathPlanning::valutazioneEuristicheUguali(int x[],int y[], int direzione1, int direzione2)
{
	double sommatoria_settore1=0;
	double sommatoria_settore2=0;
	double sommatoria_settore1_sporco=0;
	double sommatoria_settore2_sporco=0;

	std::vector<Casella> lista;

	//cout << "direzione da controllare: " << direzione1 << endl;
	//cout << "direzione massima: " << direzione2 << endl;
	mappa->getTutteCaselle(&lista);
	for(int i=0;i<(int)lista.size();i++)
	{
		//spazzolo tutta la mappa salvando le due medie
		Casella da_controllare=lista[i];
		if(da_controllare.isExist()==true)
		{
			double angolo1=this->fromDirectionToAngle(direzione1);
			double angolo2=this->fromDirectionToAngle(direzione2);

			int x_casella=da_controllare.getX();
			int y_casella=da_controllare.getY();

			int x_robot;
			int y_robot;
			this->getXY(&x_robot,&y_robot);
			//trasformazione delle coordinate del robot nel sistema di riferimento cartesiano "nostro"
			int my_position[2];
			MyUtil::coordinateFromRobotToMine(x_robot, y_robot, my_position); 
			double angolo_casella=fromCartToPolar(my_position[0],my_position[1],x_casella,y_casella);			
			//cout << "casella da controllare x = " << da_controllare.getX() << " y = " << da_controllare.getY() << endl;
			//cout << "angolo_casella: " << angolo_casella << endl;

			if(angolo1 == 180)
			{
				if(angolo_casella <= (-180+22.5)   ||  angolo_casella >= (180-22.5))
				{
					sommatoria_settore1++;
					if(da_controllare.isSporco()==true)
					{
						sommatoria_settore1_sporco++;
					}
				}
			}
			else
			{
				if(angolo_casella >= (angolo1-22.5)  &&  angolo_casella <= (angolo1+22.5))
				{
					sommatoria_settore1++;
					if(da_controllare.isSporco()==true)
					{
						sommatoria_settore1_sporco++;
					}
				}
			}
			if(angolo2 == 180)
			{
				if(angolo_casella <= (-180+22.5)   ||  angolo_casella >= (180-22.5))
				{
					sommatoria_settore2++;
					if(da_controllare.isSporco()==true)
					{
						sommatoria_settore2_sporco++;
					}
				}
			}
			else
			{
				if(angolo_casella >= (angolo2-22.5)  &&  angolo_casella <= (angolo2+22.5))
				{
					sommatoria_settore2++;
					if(da_controllare.isSporco()==true)
					{
						sommatoria_settore2_sporco++;
					}
				}
			}
		}

	}
	double media1=sommatoria_settore1_sporco/sommatoria_settore1;
	double media2=sommatoria_settore2_sporco/sommatoria_settore2;
	/*if(media2>media1&&media1>0)
	{
		return direzione1;
	}
	else
	{
		if(media2>0)
		{
			return direzione2;
		}
		return direzione1;
	}*/
	if(media2 > media1  &&  media2>0)
	{
		return direzione2;
	}
	else
	{
		if(media1>0)
		{
			return direzione1;
		}
		return direzione2;
	}
}

void MyPathPlanning::getXY(int* x, int* y)
{
	 ArPose current_position = robot->getPose();
	 *x = current_position.getX();
	 *y = current_position.getY();
}
  double MyPathPlanning::fromDirectionToAngle(int direzione)
  {
		double angolo;
		switch (direzione)
		{
			case 0:
				return 90.;
			case 1:
				return 45.;
			case 2:
				return 0.;
			case 3:
				return -45.;
			case 4:
				return -90.;
			case 5:
				return -135.;
			case 6:
				return 180.;
			case 7:
				return 135.;
		}
		/*if(direzione<3)
		{
			angolo=90-45*direzione;
		}
		else
		{
			angolo=360-45*(direzione-2);
		}*/
  }

  double MyPathPlanning::fromCartToPolar(int origin_x, int origin_y, int x, int y)
  {		
	double polar = atan2((double)(origin_y+y),(double)(origin_x+x)) * 180 / M_PI;
	if (polar == -180)
		return 180;
	  return atan2((double)(origin_y+y),(double)(origin_x+x)) * 180 / M_PI;
  }


int MyPathPlanning::approssimation2(double angle)
{
	if(angle<=(0+22.5)&&angle>=(0-22.5))
	{
		return 0;
	}
	if(angle<=(90+22.5)&&angle>=(90-22.5))
	{
		return 6;
	}
	if(angle<=(-90+22.5)&&angle>=(-90-22.5))
	{
		return 2;
	}
	if(angle<=(45+22.5)&&angle>=(45-22.5))
	{
		return 7;
	}

	if(angle<=(-45+22.5)&&angle>=(-45-22.5))
	{
		return 1;
	}
	if(angle<=(-135+22.5)&&angle>=(-135-22.5))
	{
		return 3;
	}
	if(angle<=(135+22.5)&&angle>=(135-22.5))
	{
		return 5;
	}
	return 4;



}
/*
void MyPathPlanning::MyGoto(int direzione)
{
	int angle=0;
	switch(direzione)
	{
	case 1:
		angle=45;
		break;
	case 2:
		angle=90;
		break;
	case 3:
		angle=135;
		break;
	case 4:
		angle=180;
		break;
	case 5:
		angle=-135;
		break;
	case 6:
		angle= -90;
		break;
	case 7:
		angle=-45;
		break;
	}
	robot->setDeltaHeading(angle);
	robot->move(sqrt(2*angle*angle));


	}

}
*/
