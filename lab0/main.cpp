#include "counter.h"

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
