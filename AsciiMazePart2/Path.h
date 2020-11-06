#pragma once
class Path
{
public:

    int pathG; //steps from enemy to player
    int pathH; //steps from player to evaluated square
    Path* pathParent; //Parent node
    int pathX; //coords
    int pathY;
    int pathF;

    Path(int g, int h, Path* parent, int x, int y);
    Path(int g, int h, int x, int y);
    Path();

    bool operator ==(Path* rhs);
};

