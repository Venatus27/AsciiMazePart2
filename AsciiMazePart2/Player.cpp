#include "Player.h"

using namespace std;

Player::Player(vector<Path*> r, int xStart, int yStart) {
	route = r;
	x = xStart;
	y = yStart;
	finished = false;
}
