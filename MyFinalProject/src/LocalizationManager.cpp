/*
 * LocalizationManager.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: user
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager(const OccupancyGrid& ogrid,
		Hamster* hamster) :
		ogrid(ogrid), hamster(hamster) {

}

void LocalizationManager::getRandomLocation(Particle* particle) {

	do {
		particle->x = rand() % ogrid.getWidth();
		particle->y = rand() % ogrid.getHeight();

	} while (ogrid.getCell(particle->x, particle->y) != CELL_FREE);

	particle->x = particle->x * ogrid.getResolution();
	particle->y = particle->y * ogrid.getResolution();

	particle->yaw = rand() % 360;
}

void LocalizationManager::initParticles() {

	particles.resize(PARTICLES_NUM);

	for (int i = 0; i < particles.size(); ++i) {
		particles[i] = new Particle();
		getRandomLocation(particles[i]);
	}

}

void LocalizationManager::updateParticles(double deltaX, double deltaY,
		double deltaYaw, int counter) {

	for (int i = 0; i < particles.size(); ++i) {

		Particle* particle = particles[i];
		particle->x += deltaX;
		particle->y += deltaY;
		particle->yaw += deltaYaw;

		particle->i = particle->y / ogrid.getResolution();
		particle->j = particle->x / ogrid.getResolution();

		particle->belief = computeBelief(particle);
	}

	//finding top 5 particles
	Particle* max = particles[0];
	int index = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 1; j < particles.size(); ++j) {
			if (particles[j]->belief > max->belief) {
				max = particles[j];
				index = j;

			}
		}
		particles[index]->belief = 0;
		topParticles[i] = max;
	}

	//dividing particles to groups
	for (int i = 0; i < 40; i++) {
		particles[i]->i = rand() % (100-(2*counter)) + (topParticles[0]->i - (50-counter));
		particles[i]->j = rand() % (100-(2*counter)) + (topParticles[0]->j - (50-counter));

	}
	for (int i = 40; i < 80; i++) {
		particles[i]->i = rand() % (100-(2*counter)) + (topParticles[1]->i - (50-counter));
		particles[i]->j = rand() % (100-(2*counter)) + (topParticles[1]->j - (50-counter));

	}
	for (int i = 80; i < 120; i++) {
		particles[i]->i = rand() % (100-(2*counter)) + (topParticles[2]->i - (50-counter));
		particles[i]->j = rand() % (100-(2*counter)) + (topParticles[2]->j - (50-counter));

	}
	for (int i = 120; i < 160; i++) {
		particles[i]->i = rand() % (100-(2*counter)) + (topParticles[3]->i - (50-counter));
		particles[i]->j = rand() % (100-(2*counter)) + (topParticles[3]->j - (50-counter));

	}
	for (int i = 160; i < 200; i++) {
		particles[i]->i = rand() % (100-(2*counter)) + (topParticles[4]->i - (50-counter));
		particles[i]->j = rand() % (100-(2*counter)) + (topParticles[4]->j - (50-counter));

	}

}

vector<Particle*> LocalizationManager::getParticles() {
	return this->particles;
}

double LocalizationManager::computeBelief(Particle* particle) {
	LidarScan scan = hamster->getLidarScan();

	/*int hits = 0;
	 int misses = 0;

	 for (int i = 0; i < scan.getScanSize(); i++) {
	 double angle = scan.getScanAngleIncrement() * i * DEG2RAD;
	 double dist = scan.getDistance(i);

	 int a = sin(angle) * dist;
	 int b = cos(angle) * dist;

	 int pixelX = particle->j + a;
	 int pixelY = particle->i + b;

	 if (ogrid.getCell(pixelX, pixelY) == CELL_OCCUPIED) {
	 hits++;
	 } else {
	 misses++;
	 }

	 }

	 return (float) hits / (hits + misses);*/

	int hits = 0;
	int misses = 0;

	for (int i = 0; i < scan.getScanSize(); i++) {

		double angle = scan.getScanAngleIncrement() * i * DEG2RAD;

		if (scan.getDistance(i) < scan.getMaxRange() - 0.001) {

			double obsX = particle->x
					+ scan.getDistance(i)
							* cos(
									angle + particle->yaw * DEG2RAD
											- 180 * DEG2RAD);
			double obsY = particle->y
					+ scan.getDistance(i)
							* sin(
									angle + particle->yaw * DEG2RAD
											- 180 * DEG2RAD);

			int pixelsX = obsX / ogrid.getResolution() + ogrid.getWidth() / 2;
			int pixelsY = obsY / ogrid.getResolution() + ogrid.getHeight() / 2;

			if (ogrid.getCell(pixelsX, pixelsY) == CELL_OCCUPIED) {
				hits++;
			} else {
				misses++;
			}
		}
	}

	return (float) hits / (hits + misses);
}

void LocalizationManager::printParticles() const {
	for (int i = 0; i < particles.size(); ++i) {
		Particle* particle = particles[i];

		cout << "Particle " << i << ": " << particle->j << "," << particle->i
				<< ", Belief: " << particle->belief << endl;

	}
}
LocalizationManager::~LocalizationManager() {
	// TODO Auto-generated destructor stub
}

