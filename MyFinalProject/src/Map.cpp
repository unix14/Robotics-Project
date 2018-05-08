/*
 * Map.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: user
 */

#include "Map.h"

Map::Map() {
	// TODO Auto-generated constructor stub
	 _hamster = new HamsterAPI::Hamster(1);
	 _ogrid = _hamster->getSLAMMap();

}

Map::~Map() {
	// TODO Auto-generated destructor stub
}

