#pragma once
#ifndef RULE_H
#define RULE_H

#include <vector>

class Rule {
public:
	Rule(std::vector<int>& born, std::vector<int>& survive);
	Rule();
	~Rule();
	std::vector<int> getBorn() const;
	std::vector<int> getSurvive() const;
private:
	std::vector<int> born;
	std::vector<int> survive;
};

#endif // RULE_H