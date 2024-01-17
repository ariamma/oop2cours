#pragma once
#ifndef GAME_H
#define GAME_H

#include <string>
#include "Universe.h"

class Game {
public:
    Game(const std::string& inputFilename, const std::string& outputFilename, unsigned int n);
    Game();
    ~Game();
    void run();
private:
    void processCommand(const std::string& command);
    void selectGameMode();
    void defaultMode();
    void readFileMode(const std::string& input);
    void calculateIterations(int n = 1);
    void dumpUniverseToFile(const std::string& filename);
    void printHelp() const;
    void printUniverse() const;

    std::string getOutputFilename()const;
    int getTotalIterations() const;

    std::string outputFilename;
    unsigned int totalIterations;
    Universe universe;
};

#endif // GAME_H