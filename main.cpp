#include <iostream>
#include <fstream>
#include "SudokuSolver.h"

int main() {
    std::ifstream inputFile("input.txt");
    SudokuSolver sudokuSolver;

    inputFile >> sudokuSolver;

    sudokuSolver.solve();

    std::cout << sudokuSolver;

    return 0;
}