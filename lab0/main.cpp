#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <clocale>
#include <stdexcept>

class WordCounter {
private:
    static bool IsDelimiter(char c) {
        return !std::isalnum(c);
    }
    std::map<std::string, int> wordCount;

public:
    void RemoveDelimiter(std::string word) {
        std::string result;
        for (char c : word) {
            if (!IsDelimiter(c)) {
                result += c;
            }
        }
        word = result;
    }

    void countWords(const std::string& inputFile) {
        std::ifstream file(inputFile);
        if (!file) {
            throw std::invalid_argument("Error opening input file!");
        }

        std::string line, word;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            while (iss >> word) {
                RemoveDelimiter(word);
                if (!word.empty()) {
                    ++wordCount[word];
                }
            }
        }
        file.close();
    }

    void createCSV(const std::string& outputFile) {
        std::ofstream output(outputFile);
        if (!output) {
            throw std::invalid_argument("Error opening output file!");
        }

        std::vector<std::pair<std::string, int>> wordFrequency(wordCount.begin(), wordCount.end());
        std::sort(wordFrequency.begin(), wordFrequency.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.second > rhs.second;
        });

        int totalCount = 0;
        for (const auto& pair : wordFrequency) {
            totalCount += pair.second;
        }

        output << "Word,Frequency,Frequency (%)" << std::endl;
        for (const auto& pair : wordFrequency) {
            output << pair.first << "," << pair.second << ","
                << (static_cast<double>(pair.second) / totalCount) * 100.0 << "%" << std::endl;
        }

        output.close();

        std::cout << "CSV file created successfully!" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::string inputFile;
    std::string outputFile;
    if (argc < 3) {
        std::cout << "Please, enter input file: ";
        std::cin >> inputFile;
        std::cout << "Enter output file: ";
        std::cin >> outputFile;
    }
    else {
        inputFile = argv[1];
        outputFile = argv[2];
    }
    WordCounter wordCounter;
    try {
        wordCounter.countWords(inputFile);
        wordCounter.createCSV(outputFile);
    }
    catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}