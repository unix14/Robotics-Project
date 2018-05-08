/*
 * LocalizationManager.h
 *
 *  Created on: Jan 15, 2017
 *      Author: user
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include <vector>
#include <iostream>
#include "Particle.h"
#include <HamsterAPIClientCPP/Hamster.h>
#define PARTICLES_NUM 200

using namespace std;
using namespace HamsterAPI;

class LocalizationManager {
private:

	vector<Particle*> particles;
	const OccupancyGrid& ogrid;
	Hamster* hamster;
	void getRandomLocation(Particle* particle);
	double computeBelief(Particle* particle);

public:
	vector<Particle*> getParticles();
	LocalizationManager(const OccupancyGrid& ogrid,Hamster* hamster);
	void initParticles();
	void updateParticles(double deltaX,double delatY,double deltaYaw);
	void printParticles()const;
	virtual ~LocalizationManager();
};

#endif /* LOCALIZATIONMANAGER_H_ */
