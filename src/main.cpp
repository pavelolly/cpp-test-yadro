#include <iostream>
#include <fstream>
#include <format>

#include "club_info_analyzer.hpp"

// TODO: C++ 17 compat ?

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << std::format("Usage: {} <file.txt>\n", argv[0]);
        return 1;
    }

    std::ifstream is(argv[1]);
    if (!is) {
        std::cout << std::format("Failed to open '{}'\n", argv[1]);
        return 1;
    }

    try {
        InputData data = LoadInputData(is);
        OutputData res = ProcessInputData(data);
        DumpOutputData(std::cout, res);
    } catch (InputDataFormatError &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}