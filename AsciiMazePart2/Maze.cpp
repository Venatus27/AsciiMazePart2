#include "Maze.h"
#include "Astar.h"
#include "Path.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;

void Maze::printMaze() {
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
            if (line.at(i) == 'P') {
                addToPlayers(i, height);
                pausedMaze = true;
            }
        }

        widthMade = true;
        height++;
    }

    for (auto& i : playerCoords) {
        setupPlayer(i.x, i.y);
    }

    MyReadFile.close();

    //exit registering

    for (int x = 1; x < width; x++) {
        if (map[0][x] == 'E') {
            exits++;
            addToExits(x, 0, 0);
        }

        if (map[height - 1][x] == 'E') {
            exits++;
            addToExits(x, height - 1, 2);
        }
    }

    for (int x = 1; x < height; x++) {
        if (map[x][0] == 'E') {
            exits++;
            addToExits(0, x, 3);
        }

        if (map[x][width - 1] == 'E') {
            exits++;
            addToExits(width - 1, x, 1);
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
    cout << "enter number of enterances (minimum 0, maximum: 10): ";
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

    for (auto& i : players) {
        map[i->y][i->x] = 'P';
    }

    map[middleHeight][middleWidth] = 'F';
}

void Maze::makeNewExits() {

    int exitLoop = 0;

    while (exitLoop < exits) {

        int locWidth = (rand() % (width - 2)) + 1;
        int locHeight = (rand() % (height - 2)) + 1;

        int card = rand() % 4;

        switch (card) { //choose wall: 0 = north, 1 = east, 2 = south, 3 = west
        case 0:

            if (map[1][locWidth] == ' ' && map[0][locWidth] != 'E') {
                map[0][locWidth] = 'E';
                exitLoop++;
                addToExits(locWidth, 0, card);
            }
            break;

        case 1:

            if (map[locHeight][width - 2] == ' ' && map[locHeight][width - 1] != 'E') {
                map[locHeight][width - 1] = 'E';
                exitLoop++;
                addToExits(width - 1, locHeight, card);
            }
            break;

        case 2:

            if (map[height - 2][locWidth] == ' ' && map[height - 1][locWidth] != 'E') {
                map[height - 1][locWidth] = 'E';
                exitLoop++;
                addToExits(locWidth, height - 1, card);
            }
            break;

        case 3:

            if (map[locHeight][1] == ' ' && map[locHeight][0] != 'E') {
                map[locHeight][0] = 'E';
                exitLoop++;
                addToExits(0, locHeight, card);
            }
            break;
        }
    }
}

void Maze::addToExits(int x, int y, int card) {
    Vec2 v;
    v.x = x;
    v.y = y;
    exitCoords.emplace_back(v);

    if (!pausedMaze) { //dont set up new players if it is a paused maze
        switch (card) {
        case 0: setupPlayer(x, y + 1); break;
        case 1: setupPlayer(x - 1, y); break;
        case 2: setupPlayer(x, y - 1); break;
        case 3: setupPlayer(x + 1, y); break;
        }
    }
}

void Maze::addToPlayers(int x, int y) {
    Vec2 v;
    v.x = x;
    v.y = y;
    playerCoords.emplace_back(v);
}

void Maze::AStarSearches() {

    int middleWidth = width / 2;
    int middleHeight = height / 2;

    for (int i = 0; i < exitCoords.size(); i++) {
        vector<Path*> vp = a.AStarPath(exitCoords[i].x, exitCoords[i].y, middleWidth, middleHeight, map, width, height);

        int lastPlaceX = middleWidth;

        for (auto& j : vp) {
            map[j->pathY][j->pathX] = map[j->pathY][j->pathX] == ' ' ? 'o' : map[j->pathY][j->pathX];
        }

        cout << "route for exit coordinates (" << exitCoords[i].x << ',' << exitCoords[i].y << ')' << '\n';
        printMaze();

        lastPlaceX = middleWidth;

        for (auto& j : vp) {
            map[j->pathY][j->pathX] = map[j->pathY][j->pathX] == 'o' ? ' ' : map[j->pathY][j->pathX];
            lastPlaceX = j->pathX;
            delete j;
        }
    }
}

void Maze::clearMaze() {
    width = 0;
    height = 0;
    exits = 0;

    map.clear();
    exitCoords.clear();
    for (auto& i : players) {
        delete i;
    }
    players.clear();
    playerCoords.clear();
    pausedMaze = false;
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

void Maze::setupPlayer(int x, int y) {

    int middleWidth = width / 2;
    int middleHeight = height / 2;

    vector<Path*> playerPath = a.AStarPath(middleWidth, middleHeight, x, y, map, width, height);

    Player* p = new Player(playerPath, x, y);
    players.emplace_back(p);
}

//resets players to original positions they were in when the maze was created/read in
void Maze::resetPlayers() {
    
    bool needsReset = true;
    for (auto& i : players) {
        needsReset = i->finished == false ? false : needsReset;
    }

    if (needsReset) {
        int middleWidth = width / 2;
        int middleHeight = height / 2;

        for (auto& i : players) {
            i->x = i->startX;
            i->y = i->startY;
            i->addRoute(a.AStarPath(middleWidth, middleHeight, i->startX, i->startY, map, width, height));
            i->movesTaken = 0;
        }
    }
}

void Maze::movePlayerAll(bool print) {
    resetPlayers();
    while (!allPlayersFinished() && !allPlayersDeadlocked()) {
        playerMovement(print);
    }

    bool endLock = false;
    for (auto& i : players) {
        endLock = i->locked < 2 ? endLock : true;
    }

    if (endLock) {
        cout << "The session was ended early as the maze became unsolvable for all remaining players" << '\n';
    }

    solvability();

}

void Maze::movePlayerLimit(int turns) {
    resetPlayers();
    int turnsTaken = 0;
    while (!allPlayersFinished() && !allPlayersDeadlocked() && turns > turnsTaken) {
        playerMovement(true);
        turnsTaken++;
    }

    if (allPlayersDeadlocked()) {
        cout << "The session was ended early as the maze became unsolvable for all remaining players" << '\n';
    }

    solvability();
}

void Maze::playerMovement(bool print) {
    
        for (auto& i : players) {
            if (i->finished == false) {
                Path* r = i->route[0];
                if (map[r->pathY][r->pathX] != 'P') {
                    map[i->y][i->x] = ' ';
                    map[r->pathY][r->pathX] = map[r->pathY][r->pathX] != 'F' ? 'P' : 'F';
                    i->y = r->pathY;
                    i->x = r->pathX;
                    i->route.erase(i->route.begin());
                    i->finished = i->route.empty() ? true : false;
                    i->locked = 0;
                }

                else {
                    i->locked++;
                }

                i->movesTaken = i->movesTaken + 1;
                if(print) printMaze();
            }
        }
}

//checks if all players are finished, cant use deadlock code for this job because of the condition changing when the maze ends
bool Maze::allPlayersFinished() {
    for (auto& i : players) {
        if (i->finished == false) return false;
    }

    return true;
}

bool Maze::allPlayersDeadlocked() {
    
    for (auto& i : players) {
        if (i->finished == false && i->locked < 2) return false;
    } 

    return true;
}

void Maze::solvability() {
    
    int notFinished = 0;
    for (auto& i : players) {
        notFinished = i->locked >= 2 ? notFinished + 1 : notFinished;
    }

    if (notFinished == 0) cout << "The maze was fully solvable by all players" << '\n' << '\n';

    else if (notFinished == players.size()) cout << "The maze was fully unsolvable by all players" << '\n' << '\n';

    else cout << "the maze was partially solvable" << '\n' << '\n';

}

void Maze::multiRun() {

    int movesTakenTotal = 0;
    int widthHere;
    int heightHere;
    int exitsHere;

    cout << "enter width for these 100 mazes (minimum 10, maximum: 30): ";
    widthHere = userInput(10, 30);
    cout << "enter height for these 100 mazes (minimum 10, maximum: 30): ";
    heightHere = userInput(10, 30);
    cout << "enter number of enterances for these 100 mazes (minimum 0, maximum: 10): ";
    exitsHere = userInput(0, 10);

    for (int i = 0; i < 100; i++) {
        
        clearMaze();

        width = widthHere;
        height = heightHere;
        exits = exitsHere;

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

        for (auto& i : players) {
            map[i->y][i->x] = 'P';
        }

        map[middleHeight][middleWidth] = 'F';

        movePlayerAll(false);

        for (auto& i : players) {
            movesTakenTotal += i->movesTaken;
        }
    }

    float aveMove = (float)movesTakenTotal / 100;
    cout << "the average moves taken by a player to reach the end of the maze in this configuration is: " << aveMove << '\n' << '\n';

}
