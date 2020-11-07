#pragma once

#include "Path.h"
#include "Astar.h"

class Player
{
public:
	std::vector<Path*> route;
	int x;
	int y;
	bool finished;

	Player(std::vector<Path*> route, int xStart, int yStart);
};

