/*
 * Movement.h
 *
 *  Created on: Jan 20, 2017
 *      Author: user
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#define DISTANCE_FREE 30

#include <HamsterAPIClientCPP/Hamster.h>

using namespace std;
using namespace HamsterAPI;

enum actions {
	forward = 0, backward = -1, leftWay = 1, rightWay = 2
};
class Movement {

private:
	HamsterAPI::Hamster * _hamster;
	bool isStuckFlag;
	int lastAction;
public:
	Movement(HamsterAPI::Hamster*);

	void getScansBetween(double min, double max, std::vector<double> & distances);
	bool willCollide(std::vector<double> distances, int angle_from_center);

	bool isFrontFree();
	bool isLeftFree();
	bool isRightFree();
	bool isBackFree();

	void moveForward();
	void turnLeft();
	void turnRight();
	void moveBackwards();

	void stopMoving();
	void uTurn();
	void randomMovement();

};

#endif /* MOVEMENT_H_ */