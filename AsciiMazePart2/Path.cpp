#include "Path.h"
#include <cstddef>

using namespace std;

Path::Path(int g, int h, Path* parent, int x, int y)
{
    pathG = g;
    pathH = h;
    pathParent = parent;
    pathX = x;
    pathY = y;
    pathF = g + h;
}

Path::Path(int g, int h, int x, int y)
{
    pathG = g;
    pathH = h;
    pathParent = nullptr;
    pathX = x;
    pathY = y;
    pathF = g + h;
}

Path::Path()
{
    pathG = NULL;
    pathH = NULL;
    pathParent = nullptr;
    pathX = NULL;
    pathY = NULL;
    pathF = 100000;
}

bool Path::operator==(Path* rhs) {
    if (this->pathX == rhs->pathX && this->pathY == rhs->pathY && this->pathParent == rhs->pathParent && this->pathG == rhs->pathG && this->pathH == rhs->pathH) {
        return true;
    }

    return false;
}
