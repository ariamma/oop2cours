#pragma once
#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell(int state = 0);
    int getState() const;
    void setState(const bool state);
    bool state;
};

bool operator==(const Cell& cell, const int n);

#endif // CELL_H