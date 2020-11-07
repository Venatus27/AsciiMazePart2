#include "Player.h"

using namespace std;

Player::Player(vector<Path*> r, int xStart, int yStart) {
	route = r;
	x = xStart;
	y = yStart;
	startX = xStart;
	startY = yStart;
	finished = false;
	locked = 0;
	routeSafetyCheck();
}

void Player::routeSafetyCheck() {
	finished = route.empty() ? true : false;
	locked = route.empty() ? 2 : 0;
}

void Player::addRoute(vector<Path*> r) {
	route = r;
	routeSafetyCheck();
}
