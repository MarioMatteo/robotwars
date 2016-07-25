#include <iostream>
#include "MyPathPlanning.h"
#include "Mappa.h"
#include "Aria.h"
#include "Arnl.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) 
{

	int unit=500;
	ArRobot robot;


	 Aria::init();

	  ArArgumentParser parser(&argc, argv);
	  parser.loadDefaultArguments();

	  ArRobotConnector robotConnector(&parser, &robot);
	  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);


	  // Connect to the robot, get some initial data from it such as type and name,
	  // and then load parameter files for this robot.
	  if(!robotConnector.connectRobot())
	  {
	    ArLog::log(ArLog::Terse, "lasersExample: Could not connect to the robot.");
	    if(parser.checkHelpAndWarnUnparsed())
	    {
	        // -help not given
	        Aria::logOptions();
	        Aria::exit(1);
	    }
	  }


	  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
	  {
	    Aria::logOptions();
	    Aria::exit(2);
	    return 2;
	  }

	  ArLog::log(ArLog::Normal, "lasersExample: Connected to robot.");

	  // Start the robot processing cycle running in the background.
	  // True parameter means that if the connection is lost, then the
	  // run loop ends.
	  robot.runAsync(true);


	  // Connect to laser(s) defined in parameter files, if LaseAutoConnect is true
	  // for the laser.
	  // (Some flags are available as arguments to connectLasers() to control error behavior and to control which lasers are put in the list of lasers stored by ArRobot. See docs for details.)
	  if(!laserConnector.connectLasers())
	  {
	    ArLog::log(ArLog::Terse, "Could not connect to configured lasers. Exiting.");
	    Aria::exit(3);
	    return 3;
	  }

	  // Allow some time to read laser data
	  ArUtil::sleep(500);

	  ArLog::log(ArLog::Normal, "Connected to all lasers.");

	Mappa* mappa = new Mappa(unit);
	MyPathPlanning* path_planning= new MyPathPlanning(unit,mappa, &robot);

	//cout<<"prova 1"<<endl;
	path_planning->percorri();


	 // Test funzioni mappa
/*
	Mappa* mappa = new Mappa(250);


	mappa->creaCasella(1000,500);
	mappa->occupaCasella(1000,500);
	mappa->creaCasella(-800,600);
	mappa->occupaCasella(-800,600);
	mappa->creaCasella(400,-300);
	mappa->pulisciCasella(400,-300);
	mappa->creaCasella(-500,-1000);
	mappa->pulisciCasella(-500,-1000);
	mappa->creaCasella(-50,-100);
	mappa->liberaCasella(-50,-100);
	mappa->creaCasella(-550,340);
	mappa->occupaCasella(-550,340);
	mappa->toString();
	return 0;
*/
}
