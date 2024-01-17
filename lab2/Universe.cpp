#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Universe.h"

Universe::Universe() : length(0), width(0) {
    name = "";
    rule = Rule();
    state = std::vector<std::vector<Cell>>();
}

Universe::Universe(const std::string& name, const Rule& rule, int length, int width)
    : name(name), rule(rule), length(length), width(width) {
    state = std::vector<std::vector<Cell>>(length, std::vector<Cell>(width));
}

Universe::~Universe() {}

void Universe::nextState() {
    std::vector<std::vector<Cell>> nextState(length, std::vector<Cell>(width));
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            nextState[i][j].setState(checkRule(i, j));
        }
    }
    state = nextState;
}

void Universe::setName(const std::string& name) {
    this->name = name;
}

void Universe::setState(const std::vector<std::vector<Cell>>& state) {
    this->state = state;
}

void Universe::setState(const int i, const int j, const bool state) {
    (this->state[i][j]).setState(state);
}

bool Universe::checkRule(const int k, const int l) const {
    int countNeighbors = 0;
    for (int i = k - 1; i <= k + 1; ++i) {
        int indexI = (i + length) % length;
        for (int j = l - 1; j <= l + 1; ++j) {
            int indexJ = (j + width) % width;
            if (indexI == k && indexJ == l) {
                continue;
            }
            countNeighbors += getState(indexI, indexJ);
        }
    }

    std::vector<int> born = rule.getBorn();
    std::vector<int> survive = rule.getSurvive();

    for (int i = 0; i < born.size(); i++) {
        int n = born[i];
        if (countNeighbors == n) {
          return true;
        }
    }
    for (int i = 0; i < survive.size(); i++) {
        int n = survive[i];
        if (countNeighbors == n && getState(k, l) == 1) {
          return true;
        }
    }
    return false;
}

std::string Universe::getName() const {
    return this->name;
}

Rule Universe::getRule() const {
    return this->rule;
}

std::string Universe::getRuleStr() const {
    Rule rule = this->rule;
    std::vector<int> born = rule.getBorn();
    std::vector<int> survive = rule.getSurvive();
    std::string ruleStr = "B";
    for (int n : born) {
        ruleStr += (n + '0');
    }
    ruleStr += "/S";
    for (int n : survive) {
        ruleStr += (n + '0');
    }
    return ruleStr;
}

int Universe::getLength() const {
    return this->length;
}

int Universe::getWidth() const {
    return this->width;
}

std::vector<std::vector<Cell>> Universe::getState() const {
    return this->state;
}

int Universe::getState(const int k, const int l) const {
    return state[k][l].getState();
}

void Universe::printState() const {
    std::string buffer = "";
    for (int i = 0; i < getWidth() + 1; ++i) {
        buffer += "##";
    }
    buffer += "#\n";
    for (int i = 0; i < getLength(); ++i) {
        buffer += "#|";
        for (int j = 0; j < getWidth(); ++j) {
            if (getState(i, j) == 1) {
                buffer += "+|";
            }
            else {
                buffer += " |";
            }
        }
        buffer += "#\n";
    }
    for (int i = 0; i < getWidth() + 1; ++i) {
        buffer += "##";
    }
    buffer += "#\n";
    std::cout << buffer;
}

Universe readUniverseFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    std::getline(file, line);

    if (line != "#Life 1.06") {
        throw std::runtime_error("Invalid file format");
    }

    std::getline(file, line);
    if (line.size() <= 3 || line[0] != '#' || line[1] != 'N' || line[2] != ' ') {
        throw std::runtime_error("Invalid name format");
    }
    std::string name = line.substr(3);

    std::getline(file, line);
    if (line.size() <= 3 || line[0] != '#' || line[1] != 'R' || line[2] != ' ') {
        throw std::runtime_error("Invalid rule format");
    }
    std::string ruleStr = line.substr(3);

    std::string bornStr, survivalStr;
    size_t slashPos = ruleStr.find('/');
    if (slashPos == std::string::npos || slashPos == 0 || slashPos == ruleStr.size() - 1) {
        throw std::runtime_error("Invalid rule format");
    }
    bornStr = ruleStr.substr(1, slashPos - 1);
    survivalStr = ruleStr.substr(slashPos + 1);

    std::vector<int> born, survival;
    for (char c : bornStr) {
        if (std::isdigit(c)) {
            born.push_back(c - '0');
        }
    }
    for (char c : survivalStr) {
        if (std::isdigit(c)) {
            survival.push_back(c - '0');
        }
    }

    std::getline(file, line);
    if (line.size() <= 3 || line[0] != '#' || line[1] != 'S' || line[2] != ' ') {
        throw std::runtime_error("Invalid size format");
    }
    std::string sizeStr = line.substr(3);
    std::string lengthStr, widthStr;
    size_t spacePos = sizeStr.find(' ');
    if (spacePos == std::string::npos || spacePos == 0 || spacePos == sizeStr.size() - 1) {
        throw std::runtime_error("Invalid size format");
    }
    lengthStr = sizeStr.substr(0, spacePos);
    widthStr = sizeStr.substr(spacePos + 1);

    int length = 0, width = 0;
    for (char c : lengthStr) {
        if (std::isdigit(c)) {
            length = length * 10 + (c - '0');
        }
    }
    for (char c : widthStr) {
        if (std::isdigit(c)) {
            width = width * 10 + (c - '0');
        }
    }
    
    Universe universe(name, Rule(born, survival), length, width);
    // Читаем остальные строки файла и заполняем состояние вселенной
    while (std::getline(file, line)) {
        int x, y;
        std::istringstream iss(line);
        if (!(iss >> x >> y)) {
            throw std::runtime_error("Invalid point format");
        }
        if (x >= length || y >= width) {
            throw std::runtime_error("Coordinates out of range");
        }
        universe.setState(x, y);
    }
    return universe;
}