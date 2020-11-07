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
    bool valid = false;
    string file;

    while (valid == false) {
        cout << "No Maze loaded, press the number for what you would like to do?" << '\n'
            << "[1] Generate new Maze" << '\n' << "[2] Read Maze in from default file" << '\n'
            << "[3] Read maze in from custom file" << '\n';

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

        default:
            cout << "invalid number please try again" << '\n';
        }

    }

    bool exit = false;

    while (exit == false) {
        cout << "Maze loaded, press the number for what you would like to do?" << '\n'
            << "[1] Generate new Maze" << '\n' << "[2] Read Maze in from default file" << '\n'
            << "[3] Read maze in from custom file" << '\n' << "[4] Print Maze" << '\n'
            << "[5] Show best route from all maze enterances" << '\n' << "[6] Save maze to default file"
            << '\n' << "[7] Save maze to custom file" << '\n' << "[8] Exit" << '\n';

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
            exit = true;
            break;

        case 9:
            m.movePlayer();
            break;

        default:
            cout << "invalid number please try again" << '\n';
        }
    }
}
