#pragma once
#include <vector>;
#include <fstream>;

struct Vec2 {
    int x;
    int y;
};

class Maze
{
public:

    int width;
    int height;
    int exits;
    std::vector<std::vector<char>> map;
    std::vector<Vec2> exitCoords;

    void printMaze();
    bool inBounds(int x, int y);
    void makeMaze(int x, int y);
    void writeMapFile(std::string file);
    bool readMapFile(std::string file);
    void initaliseNewMaze();
    void makeNewExits();
    void addToExits(int x, int y);
    void AStarSearches();
    void clearMaze();
    int userInput(int min, int max);
};

