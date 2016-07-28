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
	SensorReadingBox* sensor = new SensorReadingBox(mappa, robot,unit, num_cell);
	int duration =90000;
  // Collision avoidance actions at higher priority
  ArActionStallRecover recover;
  ArActionBumpers bumpers;
  ArActionLimiterForwards limiterAction("speed limiter far", unit/2, unit*2, 400);
  ArActionAvoidFront avoidFrontNear("Avoid Front Near", unit/2, 400, 45);


   robot->addAction(&recover, 100);
  robot->addAction(&bumpers, 75);
  robot->addAction(&avoidFrontNear, 50);
robot->addAction(&limiterAction, 90);
  ArActionGoto gotoPoseAction("goto");
  robot->addAction(&gotoPoseAction, 50);

  // Stop action at lower priority, so the robot stops if it has no goal
  ArActionStop stopAction("stop");
  robot->addAction(&stopAction, 40);


  // turn on the motors, turn off amigobot sounds
  robot->enableMotors();
  robot->comInt(ArCommands::SOUNDTOG, 0);




  //int goalNum = 0;
  int direction=0;
  ArTime start;
  start.setToNow();
  int previus_x=-1;
  int previus_y=-1;
  mappa->creaCasella(0,0);
  cout<<"\n inizio ciclo"<<endl;
  while (Aria::getRunning())
  {
	  robot->lock();
     int current_x;
     int current_y;

     this->getXY(&current_x,&current_y);
     sensor->readSensor();
     current_x = MyUtil::centraCoordinate(current_x,unit);
     current_y = MyUtil::centraCoordinate(current_y,unit);
     mappa->pulisciCasella(current_x,current_y);

     if(current_x!=previus_x||current_y!=previus_y)
     {
      start.setToNow();
     }
     previus_y=current_y;
     previus_x=current_x;


     int future_x;
     int future_y;
//numerazione stile quadranti orologio, la posizione delle celle locali non cambia in base alla direzione ma in base alla direzione della mappa;
    int x[8];
    int y[8];

    for(int i=0;i<8;i++)
    {
      x[i]=current_x;
      y[i]=current_y;
      if(i>=1&&i<=3)
      {
        x[i]=x[i]+unit; //per andare dall'indirizzo della cella dove sono a quelle subito vicine
      }
      if(i>=5&&i<=7)
      {
        x[i]=x[i]-unit;
      }
      if(i==7||(i>=0&&i<=1))
      {
        y[i]=y[i]+unit;
      }
      if(i>=3&&i<=5)
      {
        y[i]=y[i]-unit;
      }
    }
    ArPose current_position=robot->getPose();
    direction=this->approssimation2(current_position.getTh());
    //cout<<"direzione del robot non approssimata"<<current_position.getTh()<<endl;
    //cout<<"direzione del robot"<<direction<<endl;
    //cout<<"prima di scelta euristica"<<endl;
    int futura_direzione=this->sceltaEuristica(x,y,direction);
   // cout<<"prima di scelta euristica"<<endl;

    if(futura_direzione==-1)
    {
    	gotoPoseAction.cancelGoal();
		robot->unlock();
		ArUtil::sleep(3000);
		break;
    }

    future_x=x[futura_direzione];
    future_y=y[futura_direzione];

    direction=futura_direzione;



   Casella* casella= mappa->getCasella(future_x,future_y);
   cout<<"nuova casella"<<endl;
   cout<<casella->isOstacolo()<<endl;
   cout<<casella->isSporco()<<endl;
   int future_goal_coordinate[2];
   MyUtil::coordinateFromMineToRobot(future_x, future_y, future_goal_coordinate);
      gotoPoseAction.setGoal(ArPose(future_goal_coordinate[0], future_goal_coordinate[1]));

      int my_goal[2];
         MyUtil::coordinateFromRobotToMine(gotoPoseAction.getGoal().getX(), gotoPoseAction.getGoal().getY(), my_goal);
      ArLog::log(ArLog::Normal, "Going to next goal  at %.0f %.0f",
            my_goal[0], my_goal[1]);


    if(start.mSecSince() >= duration) {
      ArLog::log(ArLog::Normal, "%d seconds have elapsed. Cancelling current goal, waiting 3 seconds, and exiting.", duration/1000);
      gotoPoseAction.cancelGoal();
      robot->unlock();
      ArUtil::sleep(500);
      break;
    }

    robot->unlock();

    cout<<"fine ciclo"<<endl;
    ArUtil::sleep(30000);
  }
  cout<<"uscita dal ciclo"<<endl;

  // Robot disconnected or time elapsed, shut down
  Aria::exit(0);

}

//le caselle aggiornano stato occupato o meno quando ci ripasso
int MyPathPlanning::sceltaEuristica(int x[],int y[], int direzione)
{
	bool all_cleaned=true;
	std::vector<Casella>  lista;
	mappa->getTutteCaselle(&lista);
	for(int i=0;i<(int)lista.size();i++)
	  {
		//spazzolo tutta la mappa salvando le due medie

			Casella da_controllare=lista[i];
			if(da_controllare.isSporco()==true)
			{
				all_cleaned=false;
			}

	  }
	if(all_cleaned)
	{
		return -1;
	}
  int euristica_max=0;
  int indice_casella_max=0;
  int direzione_max=direzione;
  //la direzione migliore è quella di andare avanti seguendo la propria direzione che all'inizio corrisponde alla direzione 0
  for(int i=0;i<8;i++)
  {
    int euristica=0;
    int direzione_analizzata=(direzione+i)%8;
   // cout<<"casella da valutare "<<x[i]<<" , "<<y[i]<<endl;
    //cout<<"direzione, direzione analizzata "<<direzione<<" , "<<direzione_analizzata<<endl;
    Casella* casella=mappa->getCasella(x[i],y[i]);
    //cout<<"ho ricevuto la casella"<<endl;

    if(casella->isOstacolo()==false&& casella->isExist())
    {
      euristica=euristica+12;
    }
    if(casella->isExist()==false)
	 {
		 euristica=euristica+1;
	 }
    if(casella->isSporco()==true && casella->isExist())
    {

      if(direzione_analizzata==direzione)
      {
        euristica=euristica+6;
      }
      if(direzione_analizzata==(direzione+1)%8||direzione_analizzata==(direzione+7)%8)
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
			  direzione_max=this->valutazioneEuristicheUguali(x, y, direzione_max, direzione_analizzata);

		}
	}
   cout<<"euristica"<<euristica<<endl;
   cout<<"direzione "<<direzione_analizzata<<endl;
    if(direzione_analizzata==direzione_max)
    {
    	indice_casella_max=i;
    }
  }
  cout<<"euristica max"<<euristica_max<<endl;
  cout<<"direzione migliore "<<direzione_max<<endl;

  return indice_casella_max;
}

  int MyPathPlanning::valutazioneEuristicheUguali(int x[],int y[], int direzione1, int direzione2)
  {
	  double sommatoria_settore1=0;
	  double sommatoria_settore2=0;
	  double sommatoria_settore1_sporco=0;
	  double sommatoria_settore2_sporco=0;

	  std::vector<Casella> lista;

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

    		  double angolo_casella=fromCartToPolar(x_robot,y_robot,x_casella,y_casella);

    		  if((angolo1+22.5)<=angolo_casella&&(angolo1-22.5)>=angolo_casella)
    		  {
    			  sommatoria_settore1++;
    			  if(da_controllare.isSporco()==true)
    			  {
    				  sommatoria_settore1_sporco++;
    			  }
    		  }


    		  if((angolo2+22.5)<=angolo_casella&&(angolo2-22.5)>=angolo_casella)
    		  {
    			  sommatoria_settore2++;
    			  if(da_controllare.isSporco()==true)
    			  {
    				  sommatoria_settore2_sporco++;
    			  }
    		  }


    	  }

    }
    double media1=sommatoria_settore1_sporco/sommatoria_settore1;
    double media2=sommatoria_settore2_sporco/sommatoria_settore2;
    if(media2>media1&&media1>0)
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

    }

  }
void MyPathPlanning::getXY(int* x, int* y)
{
	 ArPose current_position= robot->getPose();
	 *x=current_position.getX();
	 *y=current_position.getY();
}
  double MyPathPlanning::fromDirectionToAngle(int direzione)
  {
		double angolo;
		if(direzione<3)
		{
			angolo=90-45*direzione;
		}
		else
		{
			angolo=360-45*(direzione-2);
		}
		return angolo;

  }

  double MyPathPlanning::fromCartToPolar(int origin_x, int origin_y, int x, int y)
  {
	  return atan((double)(origin_x+x)/(double)(origin_y+y));
  }


int MyPathPlanning::approssimation2(double angle)
{


	if(angle<=(0+22.5)&&angle>=(0-22.5))
	{
		return 0;
	}
	if(angle<=(90+22.5)&&angle>=(90-22.5))
	{
		return 2;
	}
	if(angle<=(-90+22.5)&&angle>=(-90-22.5))
	{
		return 6;
	}
	if(angle<=(45+22.5)&&angle>=(45-22.5))
	{
		return 1;
	}

	if(angle<=(-45+22.5)&&angle>=(-45-22.5))
	{
		return 7;
	}
	if(angle<=(-135+22.5)&&angle>=(-135-22.5))
	{
		return 5;
	}
	if(angle<=(135+22.5)&&angle>=(135-22.5))
	{
		return 3;
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
