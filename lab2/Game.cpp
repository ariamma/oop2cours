#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include "Game.h"

Game::Game(const std::string& inputFilename, const std::string& outputFilename, unsigned int n) 
    : outputFilename(outputFilename), totalIterations(n) {
    universe = readUniverseFromFile(inputFilename);
}

Game::Game()  {
    outputFilename = "";
}

Game::~Game() {}

void Game::run() {
    std::cout << "Welcome to the Game of Life!" << std::endl;
    std::cout << "Type 'help' for a list of commands." << std::endl;

    std::string command;
    while (command != "exit") {
        std::cin >> command;
        processCommand(command);
    }
}

void Game::processCommand(const std::string& command) {
    if (command == "start") {
        selectGameMode();
    } else if (command == "dump") {
        std::string filename;
        std::cin >> filename;
        dumpUniverseToFile(filename);
    } else if (command == "tick" || command == "t") {
        int n = 0;
        std::string input;
        std::cin >> input;
        if (std::cin.fail()) {
            n = 1;
        } else {
            for (char c : input) {
                n = n * 10 + (c - '0');
            }
        }
        calculateIterations(n);
        printUniverse();
    } else if (command == "help") {
        printHelp();
    } else if (command == "exit") {
        return;
    } else {
        std::cout << "Unknown command." << std::endl;
    }
}

void Game::selectGameMode() {
    std::cout << "Enter the game mode:" << std::endl;
    std::cout << "1 : Default mode" << std::endl;
    std::cout << "2 : File mode" << std::endl;
    
    int mode;
    std::cin >> mode;

    if (mode == 1) {
        defaultMode();
    } else if (mode == 2) {
        std::cout << "Enter the filename: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string input = "";
        getline(std::cin, input);
        std::cout << input << std::endl;
        readFileMode(input);
    } else {
        std::cout << "Invalid mode. Return to home page." << std::endl;
    }
}

void Game::defaultMode() {
    std::cout << "Enter the number:" << std::endl;
    std::cout << "1 : Example" << std::endl;
    std::cout << "2 : Blinker" << std::endl;
    std::cout << "3 : Pulsar" << std::endl;
    
    int mode;
    std::cin >> mode;

    Rule rule;
    if (mode == 1) {
        universe = Universe("Example", rule, 5, 5);
        universe.setState(0, 2);
        universe.setState(1, 0);
        universe.setState(1, 2);
        universe.setState(2, 1);
        universe.setState(2, 2);
        printUniverse();
    } else if (mode == 2) {
        universe = Universe("Blinker", rule, 5, 5);
        universe.setState(2, 1);
        universe.setState(2, 2);
        universe.setState(2, 3);      
        printUniverse();
    } else if (mode == 3) {
        universe = Universe("Pulsar", rule, 17, 17);
        universe.setState(2, 4);
        universe.setState(2, 5);
        universe.setState(2, 6);
        universe.setState(2, 10);
        universe.setState(2, 11);
        universe.setState(2, 12);
        universe.setState(4, 2);
        universe.setState(4, 7);
        universe.setState(4, 9);
        universe.setState(4, 14);
        universe.setState(5, 2);
        universe.setState(5, 7);
        universe.setState(5, 9);
        universe.setState(5, 14);
        universe.setState(6, 2);
        universe.setState(6, 7);
        universe.setState(6, 9);
        universe.setState(6, 14);
        universe.setState(7, 4);
        universe.setState(7, 5);
        universe.setState(7, 6);
        universe.setState(7, 10);
        universe.setState(7, 11);
        universe.setState(7, 12);
        universe.setState(9, 4);
        universe.setState(9, 5);
        universe.setState(9, 6);
        universe.setState(9, 10);
        universe.setState(9, 11);
        universe.setState(9, 12);
        universe.setState(10, 2);
        universe.setState(10, 7);
        universe.setState(10, 9);
        universe.setState(10, 14);
        universe.setState(11, 2);
        universe.setState(11, 7);
        universe.setState(11, 9);
        universe.setState(11, 14);
        universe.setState(12, 2);
        universe.setState(12, 7);
        universe.setState(12, 9);
        universe.setState(12, 14);
        universe.setState(14, 4);
        universe.setState(14, 5);
        universe.setState(14, 6);
        universe.setState(14, 10);
        universe.setState(14, 11);
        universe.setState(14, 12);
        printUniverse();
    } else {
        std::cout << "Invalid mode. Return to home page." << std::endl;
    }
}

void Game::readFileMode(const std::string& input) {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    if (tokens.size() == 1) {
        universe = readUniverseFromFile(tokens[0]);
        printUniverse();
    } else {
        universe = readUniverseFromFile(tokens[0]);
        std::string outputFilename = "";

        for (int i = 1; i < tokens.size(); ++i) {
            if (tokens[i] == "-i") {
                totalIterations = std::stoul(tokens[i+1]);
                i++;
            } else if (tokens[i].find("--iterations=") != std::string::npos) {
                std::string iterationsStr = tokens[i].substr(tokens[i].find("=") + 1);
                totalIterations = std::stoul(iterationsStr);
            } else if (tokens[i] == "-o") {
                outputFilename = tokens[i+1];
                i++;
            } else if (tokens[i].find("--output=") != std::string::npos) {
                outputFilename = tokens[i].substr(tokens[i].find("=") + 1);
            }
        }

        calculateIterations(totalIterations);
        dumpUniverseToFile(outputFilename);
    }
}

void Game::calculateIterations(int n) {
    for (int i = 0; i < n; ++i) {
        universe.nextState();
    }
}

void Game::dumpUniverseToFile(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << "#Life 1.06" << std::endl;
        outputFile << "#N " << universe.getName() << std::endl;
        outputFile << "#R " << universe.getRuleStr() << std::endl;
        outputFile << "#S " << universe.getLength() << " " << universe.getWidth() << std::endl;

        for (int i = 0; i < universe.getLength(); i++) {
            for (int j = 0; j < universe.getWidth(); j++) {
                if ((universe.getState())[i][j] == 1) {
                    outputFile << i << " " << j << std::endl;
                }
            }
        }
        outputFile.close();
        std::cout << "Universe dumped to file: " << filename << std::endl;
    } else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}

void Game::printUniverse() const {
    std::cout << "Universe: " << universe.getName() << std::endl;
    std::cout << "Rule: " << universe.getRuleStr() << std::endl;
    std::cout << "Size: " << universe.getLength() << " " << universe.getWidth() << std::endl;
    universe.printState();
}

void Game::printHelp() const {
    std::cout << "Commands:" << std::endl;
    std::cout << "start - Start the game." << std::endl;
    std::cout << "dump <filename> - Save the universe to a file." << std::endl;
    std::cout << "tick <n> (short command: t <n>) - Calculate n iterations and print the result." << std::endl;
    std::cout << "exit - Exit the game." << std::endl;
    std::cout << "help - Print help message." << std::endl;
}

std::string Game::getOutputFilename() const {
    return this->outputFilename;
}

int Game::getTotalIterations() const {
    return this->totalIterations;
}





















