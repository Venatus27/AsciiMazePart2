#include "Main.h"
#include "Maze.h"
#include <random>;
#include <chrono>;
#include <iostream>;

using namespace std;

//Main function mostly just used to handle the UI and make calls to the Maze class when required
int main() {

    srand((int)time(0));
    Maze m;
    int choice = 0;
    int turns = 0;
    bool valid = false;
    bool exit = false;
    string file;

    while (valid == false) {
        cout << "No Maze loaded, press the number for what you would like to do?" << '\n'
            << "[1] Generate new Maze" << '\n' << "[2] Read Maze in from default file" << '\n'
            << "[3] Read maze in from custom file" << '\n' << "[4] Find ave. player moves from 100 maze configurations (last instance will be loaded at end)" << '\n'
            << "[5] Exit" << '\n';

        cin >> choice;

        switch (choice) {
        case 1:
            m.initaliseNewMaze();
            valid = true;
            break;


        case 2:
            valid = m.readMapFile("mapFile.txt");
            break;

        case 3:
            cout << "please input the file name to read (e.g. mapStore.txt): ";
            cin >> file;
            valid = m.readMapFile(file);
            break;

        case 4:
            m.multiRun();
            valid = true;
            break;

        case 5:
            valid = true;
            exit = true;
            break;

        default:
            cout << "invalid number please try again" << '\n';
        }

    }


    while (exit == false) {
        cout << "Maze loaded, press the number for what you would like to do?" << '\n'
            << "[1] Generate new Maze" << '\n' << "[2] Read Maze in from default file" << '\n'
            << "[3] Read maze in from custom file" << '\n' << "[4] Print Maze" << '\n'
            << "[5] Show best route from all maze enterances" << '\n' << "[6] Save maze to default file"
            << '\n' << "[7] Save maze to custom file" << '\n' << "[8] Move all players to the finish line" << '\n'
            << "[9] Move all players a certain number of moves" << '\n' << "[10] Find ave. player moves from 100 maze configurations (last instance will be loaded at end)" << '\n'
            << "[11] Exit" << '\n';

        cin >> choice;

        switch (choice) {
        case 1:
            m.initaliseNewMaze();
            break;


        case 2:
            valid = m.readMapFile("mapFile.txt");
            break;

        case 3:
            cout << "please input the file name to read (e.g. mapStore.txt): ";
            cin >> file;
            valid = m.readMapFile(file);
            break;

        case 4:
            m.printMaze();
            break;

        case 5:
            m.AStarSearches();
            break;

        case 6:
            m.writeMapFile("mapFile.txt");
            break;

        case 7:
            cout << "please input the file name to create or edit (e.g. mapStore.txt): ";
            cin >> file;
            m.writeMapFile(file);
            break;

        case 8:
            m.movePlayerAll(true);
            break;
        
        case 9:
            cout << "please input the number of turns each player should run: ";
            cin >> turns;
            m.movePlayerLimit(turns);
            break;

        case 10:
            m.multiRun();
            break;
        
        case 11:
            exit = true;
            break;

        default:
            cout << "invalid number please try again" << '\n';
        }
    }
}
