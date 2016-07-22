/*
 * MyPathPlanning.cpp
 *
 *  Created on: 22/giu/2016
 *      Author: gabrielefcg
 */
#include "MyPathPlanning.h"




MyPathPlanning::MyPathPlanning(int unit, Mappa mappa, ArRobot robot) {
	// TODO Auto-generated constructor stub
	this->unit=unit;
	this->mappa=&mappa;
	this->robot=robot;


}

MyPathPlanning::~MyPathPlanning() {
	// TODO Auto-generated destructor stub
}




void MyPathPlanning::percorri()
{
	int duration =3000;
  // Collision avoidance actions at higher priority
  ArActionStallRecover recoverAct;
  ArActionBumpers bumpAct;
  ArActionLimiterForwards limiterAction("speed limiter far", unit/2, unit*2, 400);
  ArActionAvoidFront avoidFrontNear("Avoid Front Near", unit/2, 400, 45);


   robot.addAction(&recover, 100);
  robot.addAction(&bumpers, 75);
  robot.addAction(&avoidFrontNear, 50);
robot.addAction(&limiterAction, 90);
  ArActionGoto gotoPoseAction("goto");
  robot.addAction(&gotoPoseAction, 50);

  // Stop action at lower priority, so the robot stops if it has no goal
  ArActionStop stopAction("stop");
  robot.addAction(&stopAction, 40);


  // turn on the motors, turn off amigobot sounds
  robot.enableMotors();
  robot.comInt(ArCommands::SOUNDTOG, 0);




  int goalNum = 0;
  int direction=0;
  ArTime start;
  start.setToNow();
  int previus_x=-1;
  int previus_y=-1;
  while (Aria::getRunning())
  {
     int current_x;
     int current_y;

     this->getXY(&current_x,&current_y);
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
    ArPose current_position=robot.getPose();
    direction=this->approssimation2(current_position.getTh());
    int futura_direzione=this->sceltaEuristica(x,y,direction);

    if(futura_direzione==-1)
    {
    	gotoPoseAction.cancelGoal();
		robot.unlock();
		ArUtil::sleep(3000);
		break;
    }

    future_x=x[futura_direzione];
    future_y=y[futura_direzione];

    direction=futura_direzione;




      gotoPoseAction.getY.setGoal(ArPose(future_x, future_y));

      ArLog::log(ArLog::Normal, "Going to next goal at %.0f %.0f",
            gotoPoseAction.getGoal().getX(), gotoPoseAction.getGoal().getY());


    if(start.mSecSince() >= duration) {
      ArLog::log(ArLog::Normal, "%d seconds have elapsed. Cancelling current goal, waiting 3 seconds, and exiting.", duration/1000);
      gotoPoseAction.cancelGoal();
      robot.unlock();
      ArUtil::sleep(3000);
      break;
    }

    robot.unlock();
    ArUtil::sleep(100);
  }

  // Robot disconnected or time elapsed, shut down
  Aria::exit(0);

}

//le caselle aggiornano stato occupato o meno quando ci ripasso
int MyPathPlanning::sceltaEuristica(int x[],int y[], int direzione)
{
	bool all_cleaned=true;
	for(int row=0;row<mappa->numRow();row++)
	  {
		int numCol=mappa->getRow(row).size();
		for(int col=0;col<numCol;col++)//spazzolo tutta la mappa salvando le due medie
		{
			Casella* da_controllare=mappa->getCasella(row,col);
			if(da_controllare->isSporco()==true)
			{
				all_cleaned=false;
			}
		}
	  }
	if(all_cleaned)
	{
		return -1;
	}
  int euristica_max=0;
  int direzione_max=direzione;
  //la direzione migliore è quella di andare avanti seguendo la propria direzione che all'inizio corrisponde alla direzione 0
  for(int i=0;i<8;i++)
  {
    int euristica;
    int direzione_analizzata=(direzione+i)%8;
    Casella* casella=mappa->getCasella(x[i],y[i]);

    if(casella->isOstacolo()==false&& casella->isExist())
    {
      euristica=euristica+12;
    }
    if(casella->isSporco()==true && casella->isExist())
    {

      if(direzione_analizzata==direzione)
      {
        euristica=euristica+5;
      }
      if(direzione_analizzata==(direzione+1)%8||direzione_analizzata==(direzione+7)%8)
      {
        euristica=euristica+4;
      }
      if(direzione_analizzata==(direzione+2)%8||direzione_analizzata==(direzione+6)%8)
      {
        euristica=euristica+3;
      }
       if(direzione_analizzata==(direzione+3)%8||direzione_analizzata==(direzione+5)%8)
      {
        euristica=euristica+2;
      }
     if(direzione_analizzata==(direzione+4)%8)
      {
        euristica=euristica+1;
      }

      if(euristica>euristica_max)
      {
        euristica_max=euristica;
        direzione=direzione_analizzata;
      }
      if(euristica==euristica_max)
      {
        direzione=this->valutazioneEuristicheUguali(x, y, direzione, direzione_analizzata);

      }
    }
  }



  return direzione;
}

  int MyPathPlanning::valutazioneEuristicheUguali(int x[],int y[], int direzione1, int direzione2)
  {
	  double sommatoria_settore1=0;
	  double sommatoria_settore2=0;
	  double sommatoria_settore1_sporco=0;
	  double sommatoria_settore2_sporco=0;
    for(int row=0;row<this->mappa->numRow();row++)
    {
    	int numCol=mappa->getRow(row).size();
      for(int col=0;col<numCol;col++)//spazzolo tutta la mappa salvando le due medie
      {
    	  Casella* da_controllare=mappa->getCasella(row,col);
    	  if(da_controllare->isExist()==true)
    	  {
    		  double angolo1=this->fromDirectionToAngle(direzione1);
    		  double angolo2=this->fromDirectionToAngle(direzione2);

    		  Casella casella=mappa->getCasella(row,col);
    		  int x_casella=casella.getX();
    		  int y_casella=casella.getY();

    		  int x_robot;
			  int y_robot;
			  this->getXY(&x_robot,&y_robot);

    		  double angolo_casella=this->fromCartToPolar(x_robot,y_robot,x_casella,y_casella);

    		  if((angolo1+22.5)<=angolo_casella&&(angolo1-22.5)>=angolo_casella)
    		  {
    			  sommatoria_settore1++;
    			  if(da_controllare->isSporco()==true)
    			  {
    				  sommatoria_settore1_sporco++;
    			  }
    		  }


    		  if((angolo2+22.5)<=angolo_casella&&(angolo2-22.5)>=angolo_casella)
    		  {
    			  sommatoria_settore2++;
    			  if(da_controllare->isSporco()==true)
    			  {
    				  sommatoria_settore2_sporco++;
    			  }
    		  }


    	  }
      }
    }
    double media1=sommatoria_settore1_sporco/sommatoria_settore1;
    double media2=sommatoria_settore2_sporco/sommatoria_settore2;
    if(media1>media2&&media2>0)
    {
    	return direzione2;
    }
    else
    {
    	return direzione1;
    }

  }
void MyPathPlanning::getXY(int* x, int* y)
{
	 ArPose current_position= robot.getPose();
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

  double fromCartToPolar(int origin_x, int origin_y, int x, int y)
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
