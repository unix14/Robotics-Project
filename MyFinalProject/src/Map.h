/*
 * Map.h
 *
 *  Created on: Jan 16, 2017
 *      Author: user
 */

#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>

using namespace std;
using namespace HamsterAPI;

class Map {
private:
	OccupancyGrid _ogrid;
	HamsterAPI::Hamster * _hamster;

public:
	Map();

	virtual ~Map();
};

#endif /* MAP_H_ */
