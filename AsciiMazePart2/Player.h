#pragma once

#include "Path.h"
#include "Astar.h"

class Player
{
public:
	std::vector<Path*> route;
	int x;
	int y;
	int startX;
	int startY;
	bool finished;
	int locked;
	int movesTaken;

	Player(std::vector<Path*> route, int xStart, int yStart);

	void routeSafetyCheck();
	void addRoute(std::vector<Path*> r);
};

