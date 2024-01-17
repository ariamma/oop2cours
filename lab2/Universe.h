#pragma once
#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <string>
#include <vector>
#include "Cell.h"
#include "Rule.h"

class Universe {
public:
    Universe();
    explicit Universe(const std::string& name, const Rule& rule, int length, int width);
    ~Universe();

    void nextState();
    void setName(const std::string& name);
    void setState(const std::vector<std::vector<Cell>>& state);
    void setState(const int i, const int j, const bool n = 1);
    bool checkRule(const int k, const int l) const;

    std::string getName() const;
    Rule getRule() const;
    std::string getRuleStr() const;
    int getLength() const;
    int getWidth() const;
    std::vector<std::vector<Cell>> getState() const;
    int getState(const int k, const int l) const;
    void printState() const;
    
private:
    std::string name;
    Rule rule;
    int length;
    int width;
    std::vector<std::vector<Cell>> state;
};

Universe readUniverseFromFile(const std::string& filename);

#endif // UNIVERSE_H