#include "Rule.h"

Rule::Rule(std::vector<int>& born, std::vector<int>& survive) : born(born), survive(survive) {}

Rule::Rule() : born({3}), survive({2, 3}) {}

Rule::~Rule() {}

std::vector<int> Rule::getBorn() const {
	return born;
}

std::vector<int> Rule::getSurvive() const {
	return survive;
}