#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <stack>
// Define arrows keys for movement
#define UP 72 // Up arrow key
#define DOWN 80 // Down arrow key
#define LEFT 75 // Left arrow key
#define RIGHT 77 // Right arrow key
#define UNDO 122 // Z key for undo
int playerX, playerY; // Player's current position
std::stack<std::pair<int, int>> moveHistory; // To store player's move history for undo operation
std::vector<std::string> map;
bool init(std::string filenames) {
    // Create autosave.txt from 8by8.txt with "#" as wall arounds
    // append one line above first and one below last with "#" equal to the amount of character in first line plus 2
    // append "#" at start and end of each line
    std::ifstream inputFile(filenames);
    if (!inputFile.is_open()) {
        std::cerr << "could not read or locate 8by8.txt" << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string tempLine;
    while (getline(inputFile, tempLine)) {
        lines.push_back(tempLine);
    }
    inputFile.close();

    if (lines.empty()) {
        std::cout << "Input file '8by8.txt' is empty." << std::endl;
        return false;
    }

    std::ofstream outputFile("autosave.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not create autosave.txt" << std::endl;
        return false;
    }

    size_t firstLineLength = lines[0].length();
    std::string border(firstLineLength + 2, '#');
    outputFile << border << std::endl;
    for (const std::string& originalLine : lines) {
        outputFile << '#' << originalLine << '#' << std::endl;
    }
    outputFile << border << std::endl;
    outputFile.close();

    // Stage 2: Read the newly created map into the global 'map' vector.
    std::ifstream mapFile("autosave.txt");
    if (!mapFile.is_open()) {
        std::cerr << "Could not read or locate autosave.txt" << std::endl;
        return false;
    }

    map.clear(); // Ensure the map is empty before loading
    std::string mapLine;
    int currentY = 0;
    while (getline(mapFile, mapLine)) {
        map.push_back(mapLine);
        // Find the player's starting position
        size_t playerPos = mapLine.find('P');
        if (playerPos != std::string::npos) {
            playerX = playerPos;
            playerY = currentY;
        }
        currentY++;
    }
    mapFile.close();

    return true;
}
void undoMove() {
    if (!moveHistory.empty()) {
        std::pair<int, int> lastPos = moveHistory.top();
        moveHistory.pop();

        map[playerY][playerX] = ' ';
        playerX = lastPos.first;
        playerY = lastPos.second;
        map[playerY][playerX] = 'P';
    }
}
void clean() {
    // clean the console :))
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void render() {
    //  print map from autosave.txt to console
    for (const std::string& line : map) {
        std::cout << line << std::endl;
    }
}
void gameLoop(bool &win, bool &alive) {
        while (alive && !win) {
        // in each loop clean render , recive input - > process -> clean - > new render
        // # is wall
        // P is player
        // T is trap
        // E is exit
        // Space is walkable area
        // After every input press enter :))
        // Too lazy and too late to research other medthods :))
        render();
        int input;
        input = _getch();
        clean();
        int nextX = playerX;
        int nextY = playerY;

        if (input == 224){
            switch(_getch()) {
                case UP:
                    nextY--;
                    break;
                case DOWN:
                    nextY++;
                    break;
                case LEFT:
                    nextX--;
                    break;
                case RIGHT:
                    nextX++;
                    break;
                default:
                    std::cout << "Invalid input." << std::endl;
                    continue;
            }
        }
        else if (input == UNDO) {
            undoMove();
            continue;
        }
        else {
            std::cout << "Invalid input." << std::endl;
            continue;
        }
        if (nextY >= 0 && nextY < map.size() && nextX >= 0 && nextX < map[nextY].length()) {
            char targetCell = map[nextY][nextX];

            if (targetCell != '#') {
                moveHistory.push({playerX, playerY});
                map[playerY][playerX] = ' ';
                playerX = nextX;
                playerY = nextY;
                map[playerY][playerX] = 'P';
                if (targetCell == 'E') {
                    win = true;
                }
                else if (targetCell == 'T') {
                    alive = false;
                }
            }
        }
    }
}
int mapselectionMenu(int& map) {
    while (true) {
        std::cout << "Welcome to the Maze Game!" << std::endl;
        std::cout << "Use W A S D to move up left down right respectively." << std::endl;
        std::cout << "Reach 'E' to win, avoid 'T' to stay alive!" << std::endl;
        std::cout << "Select a map to play:" << std::endl;
        std::cout << "1. 8 by 8" << std::endl;
        std::cout << "2. 10 by 10" << std::endl;
        std::cout << "3. 12 by 12" << std::endl;
        std::cin >> map;
        if (map == 1) {
            break;
        } 
        else if (map == 2) {
            break;
        } 
        else if (map == 3) {
            break;
        } 
        else {
            clean();
            std::cout << "Invalid selection." << std::endl;
        }
    }
    return 0;
}
std::string getMapFilename(int map) {
    switch (map) {
        case 1:
            return "8by8.txt";
        case 2:
            return "10by10.txt";
        case 3:
            return "12by12.txt";
    }
    return 0;
}
int main() {
    bool win = false;
    bool alive = true;
    std::string mapselection;
    int map;
    mapselectionMenu(map);
    if (!init(getMapFilename(map))) {
        return 1; // Exit if initialization fails
    }
    clean();
    gameLoop(win, alive);
    render();
    if (win) {
        std::cout << "\nVictory!" << std::endl;
    } 
    else {
        std::cout << "\nOops, fell into a trap now dead." << std::endl;
    }

    return 0;
}
