/*
 * Robot.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: user
 */

#include "Robot.h"

Robot::Robot(Hamster* hamster):hamster(hamster),currX(0),currY(0),currYaw(0),prevX(0),prevY(0),prevYaw(0) {
	// TODO Auto-generated constructor stub
	updatePose();

}

double Robot::getDeltaX()const{
	return currX-prevX;

}
double Robot::getDeltaY()const{
	return currY-prevY;

}
double Robot::getDeltaYaw()const{
	return currYaw-prevYaw;
}
void Robot::updatePose(){

	//the location of the Robot with X,Y
	Pose pose = hamster->getPose();


	//this is to find the delta and then u create the helkikim
	prevX = currX;
	prevY = currY;
	prevYaw = currYaw;

	currX = pose.getX();
	currY = pose.getY();
	currYaw = pose.getHeading();

}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

