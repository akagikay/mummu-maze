#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <conio.h>
// Define arrows keys for movement
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
int playerX, playerY;

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

int main() {
    bool win = false;
    bool alive = true;
    int input;
    int mapselection;
    std::cout << "Welcome to the Maze Game!" << std::endl;
    std::cout << "Use W A S D to move up left down right respectively." << std::endl;
    std::cout << "Reach 'E' to win, avoid 'T' to stay alive!" << std::endl;
    std::cout << "Select a map to play:" << std::endl;
    std::cout << "1. 8 by 8" << std::endl;
    std::cout << "2. 10 by 10" << std::endl;
    std::cout << "3. 12 by 12" << std::endl;
    std::cin >> mapselection;
    if (mapselection == 1) {
        std::cout << "You have selected the 8 by 8 map." << std::endl;
        if (!init("8by8.txt")) {
            return 1; // Exit if initialization fails
        }
    } 
    else if (mapselection == 2) {
        std::cout << "You have selected the 10 by 10 map." << std::endl;
        if (!init("10by10.txt")) {
            return 1; // Exit if initialization fails
        }
    } 
    else if (mapselection == 3) {
        std::cout << "You have selected the 12 by 12 map." << std::endl;
        if (!init("12by12.txt")) {
            return 1; // Exit if initialization fails
        }
    } 
    else {
        std::cout << "Invalid selection. Defaulting to 8 by 8 map." << std::endl;
        mapselection = 1;
    }  
    clean();
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
        else {
            std::cout << "Invalid input." << std::endl;
            continue;
        }
        if (nextY >= 0 && nextY < map.size() && nextX >= 0 && nextX < map[nextY].length()) {
            char targetCell = map[nextY][nextX];

            if (targetCell == ' '){
                map[playerY][playerX] = ' ';
                playerX = nextX;
                playerY = nextY;
                map[playerY][playerX] = 'P';
            }
            else if (targetCell == 'E') {
                win = true;
                map[playerY][playerX] = ' ';
                map[nextY][nextX] = 'P';
            } 
            else if (targetCell == 'T') {
                alive = false;
                map[playerY][playerX] = ' ';
                map[nextY][nextX] = 'P';
            }        
        }
    }
    clean();
    render();
    if (win) {
        std::cout << "\nVictory!" << std::endl;
    } 
    else {
        std::cout << "\nOops, fell into a trap now dead." << std::endl;
    }

    return 0;
}
