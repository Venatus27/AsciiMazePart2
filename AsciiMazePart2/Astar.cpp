#include "Astar.h"
#include "Path.h"
#include <vector>
#include <cstddef>
using namespace std;

vector<Path*> Astar::AStarPath(int destinationX, int destinationY, int startX, int startY, vector<vector<char>> map, int width, int height) {

    vector<Path*> evaluationList;
    vector<Path*> closedPathList;

    Path* destinationSquare = new Path(0, 0, destinationX, destinationY);

    evaluationList.insert(evaluationList.begin(), new Path(0, BlocksToTarget(startX, startY, destinationX, destinationY), startX, startY));
    Path* currentSquare = new Path();

    while (evaluationList.size() > 0)
    {
        currentSquare = itemWithLowestFScore(evaluationList);

        closedPathList.insert(closedPathList.begin(), currentSquare);
        evaluationList.erase(std::remove(evaluationList.begin(), evaluationList.end(), currentSquare), evaluationList.end());

        if (doesPathListContain(closedPathList, *destinationSquare))
        {
            delete destinationSquare;
            return buildPath(currentSquare);
            break;
        }

        vector<Path*> adjacentSquares = GetAdjacentSquares(currentSquare, width, height, map, destinationX, destinationY);

        for (int i = 0; i < adjacentSquares.size(); i++)
        {
            if (doesPathListContain(closedPathList, *adjacentSquares[i]))
            {
                continue;
            }

            if (!doesPathListContain(evaluationList, *adjacentSquares[i]))
            {
                evaluationList.insert(evaluationList.begin(), adjacentSquares[i]);
            }

            else if (adjacentSquares[i]->pathH + currentSquare->pathG + 1 < adjacentSquares[i]->pathF)
            {
                adjacentSquares[i]->pathParent = currentSquare;
            }
        }

        vector<Path*>().swap(adjacentSquares);
    }

    return evaluationList; //bug fixed: empty, guards against null errors that occur on partially solvable solutions running multiple times
}
