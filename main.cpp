#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

int playerX, playerY;

std::vector<std::string> map;

bool init() {
    // Create autosave.txt from 8by8.txt with "#" as wall arounds
    // append one line above first and one below last with "#" equal to the amount of character in first line plus 2
    // append "#" at start and end of each line
    std::ifstream inputFile("8by8.txt");
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

    std::ifstream mapFile("autosave.txt");
    if (!mapFile.is_open()) {
        std::cerr << "Could not read or locate autosave.txt" << std::endl;
        return false;
    }

    map.clear();
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
    std::string input;
    if (!init()) {
        std::cout << "An error occurred during initialization." << std::endl;
        return 1;
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
        std::cin >> input;
        clean();
        int nextX = playerX;
        int nextY = playerY;

        if (input == "w") {
            nextY--;
        } 
        else if (input == "a") {
            nextX--;
        } 
        else if (input == "s") {
            nextY++;
        } 
        else if (input == "d") {
            nextX++;
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
