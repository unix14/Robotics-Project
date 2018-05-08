/*
 * Robot.h
 *
 *  Created on: Jan 15, 2017
 *      Author: user
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <HamsterAPIClientCPP/Hamster.h>

using namespace HamsterAPI;

class Robot {
private:
	double currX,currY,currYaw;
	double prevX,prevY,prevYaw;
	double DeltaX,DeltaY,DeltaYaw;
	Hamster* hamster;

public:
	Robot(Hamster* hamster);

	//Pose include the zavit
	void updatePose();
	double getDeltaX()const;
	double getDeltaY()const;
	double getDeltaYaw()const;
	virtual ~Robot();
};

#endif /* ROBOT_H_ */
