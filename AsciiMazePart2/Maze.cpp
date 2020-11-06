#include "Maze.h"
#include "Astar.h"
#include "Path.h"
#include <iostream>;
#include <fstream>
#include <vector>;
#include <random>;
#include <ctime>;
#include <chrono>;
#include <string>;
#include <sstream>

using namespace std;

void Maze::printMaze() {
    //printing
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;
}

bool Maze::inBounds(int x, int y) {
    if (x <= 0 || x >= width - 1) return false;
    if (y <= 0 || y >= height - 1) return false;

    return true;
}

void Maze::makeMaze(int x, int y) {

    map[y][x] = ' ';

    int cards[4]; //north, east, south, west in that order
    cards[0] = 0;
    cards[1] = 1;
    cards[2] = 2;
    cards[3] = 3;

    int n = sizeof(cards) / sizeof(cards[0]);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle(cards, cards + n, default_random_engine(seed));

    for (int i = 0; i < n; ++i) {
        int dx = 0;
        int dy = 0;

        switch (cards[i]) {
        case 0: dy = -1;
            break;
        case 1: dx = 1;
            break;
        case 2: dy = 1;
            break;
        case 3: dx = -1;
            break;
        }

        int x2 = x + (dx * 2);
        int y2 = y + (dy * 2);

        if (inBounds(x2, y2)) {
            if (map[y2][x2] == 'X') {
                map[y2 - dy][x2 - dx] = ' ';
                makeMaze(x2, y2);
            }
        }
    }
}

void Maze::writeMapFile(string file) {

    ofstream MyFile(file);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            MyFile << map[y][x];
        }

        MyFile << '\n';
    }

    MyFile.close();

}

bool Maze::readMapFile(string file) {

    clearMaze();

    ifstream MyReadFile;

    string line;
    MyReadFile.open(file);
    bool widthMade = false;
    int y = 0;

    if (MyReadFile.fail()) {
        cout << "Error opening save file" << '\n';
        return false;
    }

    while (getline(MyReadFile, line)) {

        width = widthMade == false ? line.size() : width;
        vector<char> row;
        map.push_back(row);

        for (int i = 0; i < width; i++) {
            map[height].push_back(line.at(i));
        }

        widthMade = true;
        height++;
    }

    MyReadFile.close();

    //exit registering

    for (int x = 1; x < width; x++) {
        if (map[0][x] == 'E') {
            exits++;
            addToExits(x, 0);
        }

        if (map[height - 1][x] == 'E') {
            exits++;
            addToExits(x, height - 1);
        }
    }

    for (int x = 1; x < height; x++) {
        if (map[x][0] == 'E') {
            exits++;
            addToExits(0, x);
        }

        if (map[x][width - 1] == 'E') {
            exits++;
            addToExits(width - 1, x);
        }
    }

    return true;
}

void Maze::initaliseNewMaze() {

    clearMaze();

    cout << "enter width (minimum 10, maximum: 30): ";
    width = userInput(10, 30);
    cout << "enter height (minimum 10, maximum: 30): ";
    height = userInput(10, 30);
    cout << "enter number of exits (minimum 0, maximum: 10): ";
    exits = userInput(0, 10);

    vector<vector<char>> build(height, vector<char>(width, 'X'));
    map = build;

    int middleWidth = width / 2;
    int middleHeight = height / 2;

    makeMaze(middleWidth, middleHeight);

    //dealing with double outer walls that appear occassionally and prevent exits

    for (int x = 1; x < width; x++) {
        if (map[1][x] == 'X' && map[2][x] != 'X') map[1][x] = ' ';
        if (map[height - 2][x] == 'X' && map[height - 3][x] != 'X') map[height - 2][x] = ' ';
    }

    for (int x = 1; x < height; x++) {
        if (map[x][1] == 'X' && map[x][2] != 'X') map[x][1] = ' ';
        if (map[x][width - 2] == 'X' && map[x][width - 3] != 'X') map[x][width - 2] = ' ';
    }

    //centre square 
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            map[middleHeight + x][middleWidth + y] = ' ';
        }
    }

    makeNewExits();

    map[middleHeight][middleWidth] = 'S';
}

void Maze::makeNewExits() {

    int exitLoop = 0;

    while (exitLoop < exits) {

        int locWidth = (rand() % (width - 2)) + 1;
        int locHeight = (rand() % (height - 2)) + 1;

        switch (rand() % 4) { //choose wall: 0 = north, 1 = east, 2 = south, 3 = west
        case 0:

            if (map[1][locWidth] == ' ' && map[0][locWidth] != 'E') {
                map[0][locWidth] = 'E';
                exitLoop++;
                addToExits(locWidth, 0);
            }
            break;

        case 1:

            if (map[locHeight][width - 2] == ' ' && map[locHeight][width - 1] != 'E') {
                map[locHeight][width - 1] = 'E';
                exitLoop++;
                addToExits(width - 1, locHeight);
            }
            break;

        case 2:

            if (map[height - 2][locWidth] == ' ' && map[height - 1][locWidth] != 'E') {
                map[height - 1][locWidth] = 'E';
                exitLoop++;
                addToExits(locWidth, height - 1);
            }
            break;

        case 3:

            if (map[locHeight][1] == ' ' && map[locHeight][0] != 'E') {
                map[locHeight][0] = 'E';
                exitLoop++;
                addToExits(0, locHeight);
            }
            break;
        }
    }
}

void Maze::addToExits(int x, int y) {
    Vec2 v;
    v.x = x;
    v.y = y;
    exitCoords.emplace_back(v);
}

void Maze::AStarSearches() {

    int middleWidth = width / 2;
    int middleHeight = height / 2;

    for (int i = 0; i < exitCoords.size(); i++) {
        Astar a;
        vector<Path*> vp = a.AStarPath(exitCoords[i].x, exitCoords[i].y, middleWidth, middleHeight, map, width, height);

        int lastPlaceX = middleWidth;
        char route = '-';

        for (auto& j : vp) {
            map[j->pathY][j->pathX] = map[j->pathY][j->pathX] == 'E' ? 'E' : 'o';
        }

        cout << "route for exit coordinates (" << exitCoords[i].x << ',' << exitCoords[i].y << ')' << '\n';
        printMaze();

        lastPlaceX = middleWidth;
        route = ' ';

        for (auto& j : vp) {
            map[j->pathY][j->pathX] = map[j->pathY][j->pathX] == 'E' ? 'E' : route;
            lastPlaceX = j->pathX;
        }
    }
}

void Maze::clearMaze() {
    width = 0;
    height = 0;
    exits = 0;
    map.clear();
    exitCoords.clear();
}

//input handling
int Maze::userInput(int min, int max) {

    int result;
    bool valid = false;

    while (valid == false) {
        cin >> result;
        if (result > max || result < min) {
            cout << "out of range value please try again" << '\n';
        }
        else {
            valid = true;
            return result;
        }
    }
}
