#include "Cell.h"

Cell::Cell(int state) : state(state) {}

int Cell::getState() const {
    return state;
}

void Cell::setState(const bool state) {
    this->state = state;
}

bool operator==(const Cell& cell, int n) {
    return cell.getState() == n;
}