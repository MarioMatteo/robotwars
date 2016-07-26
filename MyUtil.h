/*
 * MyUtil.h
 *
 *  Created on: 26/lug/2016
 *      Author: gabrielefcg
 */

#ifndef MYUTIL_H_
#define MYUTIL_H_

#include <math.h>

class MyUtil {
public:
	MyUtil();
	virtual ~MyUtil();
	//static double fromCartToPolar(int origin_x, int origin_y, int x, int y);
	//static int approssimation(double angle);
	static int centraCoordinate(int x, int unit);
};

#endif /* MYUTIL_H_ */
