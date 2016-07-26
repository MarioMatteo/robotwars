/*
 * MyUtil.cpp
 *
 *  Created on: 26/lug/2016
 *      Author: gabrielefcg
 */

#include "MyUtil.h"

MyUtil::MyUtil() {
	// TODO Auto-generated constructor stub

}

MyUtil::~MyUtil() {
	// TODO Auto-generated destructor stub
}
/*
//Approssima l'angolo di orientamento del robot con gli angoli delle nostre 8 possibili direzioni
int MyUtil::approssimation(double angle)
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
*/
int MyUtil::centraCoordinate(int x, int unit)
{
	int risultato;
	int temp=x%(unit);
	if(temp < (unit/2))
	{
		risultato= x-temp;
	}
	else
		risultato =(floor(x/unit)+1)*unit;

	return risultato;
}
/*
double MyUtil::fromCartToPolar(int origin_x, int origin_y, int x, int y)
{
  return atan((double)(origin_x+x)/(double)(origin_y+y));
}
*/
