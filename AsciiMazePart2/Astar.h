#pragma once
#include <vector>
#include <cmath>  
#include "Path.h"
#include "main.h"

using namespace std;

class Astar
{
public:
	vector<Path*> AStarPath(int DestinationX, int destinationY, int startX, int startY, vector<vector<char>> map, int width, int height);

private:

	vector<Path*> buildPath(Path* p) {
		vector<Path*> bestPath;
		Path* current = p;
		Path* lastMove;

		bestPath.insert(bestPath.begin(), current);
		while (current->pathParent != nullptr) {
			current = current->pathParent;

			if (current->pathParent != nullptr) bestPath.insert(bestPath.begin(), current);
			else lastMove = current;
		}
		
		free(current);
		return bestPath;
	};

	bool doesPathListContain(vector<Path*> l, Path p) {

		for (int i = 0; i < l.size(); i++) {
			if (l[i]->pathX == p.pathX && l[i]->pathY == p.pathY) return true;
		}

		return false;
	};

	Path* itemWithLowestFScore(vector<Path*> l) {

		int lowest = 10000;
		Path* node = new Path();

		for (auto& i : l) {
			if (i->pathF <= lowest) { //bug fixed: has to be <= otherwise the algoritm will rarely take a couple of extra steps in specific scenarios
				lowest = i->pathF;
				node = i;
			}
		}

		return node;
	}

	vector<Path*> GetAdjacentSquares(Path* p, int width, int height, vector<vector<char>> map, int destinationX, int destinationY) {

		vector<Path*> ret;

		int px = p->pathX;
		int py = p->pathY;

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int x = px + i;
				int y = py + j;

				//no diagonals or self square analysis
				if ((i == 0 && j == 0) || (i != 0 && j != 0))
				{
					continue;
				}

				else if (x < width && y < height && x >= 0 && y >= 0 && map[py][px] != 'X')
				{
					Path* newP = new Path(p->pathG + 1, BlocksToTarget(x, y, destinationX, destinationY), p, x, y);
					ret.insert(ret.begin(), newP);

				}

			}
		}

		return ret;
	}

	int BlocksToTarget(int currentX, int currentY, int targetX, int targetY)
	{
		int final = (abs(currentX - targetX) + abs(currentY - targetY));
		return final;
	}
};
