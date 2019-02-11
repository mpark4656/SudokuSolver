//
// Created by mpark on 2/10/19.
//
#include "SudokuSolver.h"

/**
 * The size of a Sudoku board is always 9 X 9
 */
const std::size_t SudokuSolver::BOARD_SIZE = 9;

/**
 * Holds possible range of numbers to write on the Sudoku board
 */
const std::array<int, 9> SudokuSolver::possibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};

/**
 * Region Constructor
 * @param lx std::size_t low x-coordinate boundary
 * @param hx std::size_t high x-coordinate boundary
 * @param ly std::size_t low y-coordinate boundary
 * @param hy std::size_t high y-coordinate boundary
 */
SudokuSolver::Region::Region(std::size_t lx, std::size_t hx, std::size_t ly, std::size_t hy)
: lowX{ lx }, highX{ hx }, lowY { ly }, highY{ hy }
{}

/**
 * Given x and y coordinates, determine if the coordinates are in this region
 * @param x std::size_t x-coordinate
 * @param y std::size_t y-coordinate
 * @return bool true if this region contains the given coordinates
 */
bool SudokuSolver::Region::contains(std::size_t x, std::size_t y) const {
    return x >= lowX && x < highX && y >= lowY && y < highY;
}

/**
 * Default Constructor
 */
SudokuSolver::SudokuSolver()
: board{ BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0) }, solved{ false }, initialized{ false }
{
    // Define the lower & upper bounds of all 9 regions
    regions.emplace_back(Region(0, 3, 0, 3));
    regions.emplace_back(Region(0, 3, 3, 6));
    regions.emplace_back(Region(0, 3, 6, 9));
    regions.emplace_back(Region(3, 6, 0, 3));
    regions.emplace_back(Region(3, 6, 3, 6));
    regions.emplace_back(Region(3, 6, 6, 9));
    regions.emplace_back(Region(6, 9, 0, 3));
    regions.emplace_back(Region(6, 9, 3, 6));
    regions.emplace_back(Region(6, 9, 6, 9));
}

/**
 * Solve this Sudoku problem
 * @pre initialized is true
 * @post solved is set to true
 */
void SudokuSolver::solve() {
    solved = initialized && solve(0);
}

/**
 * Recursive function to solve the Sudoku puzzle using backtracking
 * @param int Only exists to differentiate 2 functions with the same name
 * @return bool true if solved
 */
bool SudokuSolver::solve(int) {
    std::size_t x;
    std::size_t y;

    // Base Case - Return true if the Sudoku board is complete
    if(isComplete(x, y)) return true;

    // Try plugging each of the possible values in the unsolved square
    for(const auto & value : possibleValues) {
        if(isValid(x, y, value)) {
            // Set the value to this unsolved square
            board.at(x).at(y) = value;

            // If this plugged value leads to the puzzle being solved, return true
            if(solve(0)) {
                return true;
            }

            // Backtrack
            board.at(x).at(y) = 0;
        }
    }

    return false;
}

/**
 * Determine if the Sudoku board is complete.
 * If the board is complete, set x and y to -1.
 * If the board is not complete, set the first unsolved x and y coordinates.
 * @param x std::size_t & x-coordinate
 * @param y std::size_t & y-coordinate
 * @return bool true if the Sudoku board is complete
 */
bool SudokuSolver::isComplete(std::size_t & x, std::size_t & y) const {
    // Find the first unsolved square
    for(std::size_t i = 0; i < board.size(); i++) {
        for(std::size_t j = 0; j < board.at(i).size(); j++) {
            if(board.at(i).at(j) == 0) {
                x = i;
                y = j;
                return false;
            }
        }
    }
    x = INT32_MAX;
    y = INT32_MAX;
    return true;
}

/**
 * Check if the value at the given coordinates is valid
 * @param x std::size_t x-coordinate
 * @param y std::size_t y-coordinate
 * @param value int value
 * @return bool true if the value is valid
 */
bool SudokuSolver::isValid(std::size_t x, std::size_t y, int value) const {
    // Check horizontal and vertical for duplicates
    for(std::size_t i = 0; i < board.size(); i++) {
        if(board.at(i).at(y) == value || board.at(x).at(i) == value) {
            return false;
        }
    }

    // Search one of the 9 regions for duplicates
    Region region = getRegion(x, y);
    for(std::size_t i = region.lowX; i < region.highX; i++) {
        for(std::size_t j = region.lowY; j < region.highY; j++) {
            if(board.at(i).at(j) == value) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Determine which region the given coordinates are located
 * @param x std::size_t x-coordinate
 * @param y std::size_t y-coordinate
 * @return Region & Region object
 */
const SudokuSolver::Region & SudokuSolver::getRegion(std::size_t x, std::size_t y) const {
    for(const auto & region : regions) {
        if(region.contains(x, y)) {
            return region;
        }
    }
}

/**
 * Read from the input stream and initialize the Sudoku board.
 * @param is std::istream & input stream
 * @param sudokuSolver SudokuSolver & SudokuSolver object
 * @post initialized is set to true
 * @post solved is set to false
 * @return std::istream & input stream
 */
std::istream & operator>>(std::istream & is, SudokuSolver & sudokuSolver) {
    for(auto & row : sudokuSolver.board) {
        for(auto & cell : row) {
            is >> cell;
        }
    }
    sudokuSolver.initialized = true;
    sudokuSolver.solved = false;
    return is;
}

/**
 * Print the solved Sudoku board
 * @param os
 * @param sudokuSolver
 * @pre initialized is true
 * @pre solved is true
 * @return
 */
std::ostream & operator<<(std::ostream & os, const SudokuSolver & sudokuSolver) {
    if(sudokuSolver.initialized && sudokuSolver.solved) {
        for(auto row : sudokuSolver.board) {
            for(auto cell : row) {
                os << cell << " ";
            }
            os << std::endl;
        }
    } else if(sudokuSolver.initialized) {
        os << "The Soduku puzzle has not been solved..." << std::endl;
    } else {
        os << "The Sudoku board has not been initialized..." << std::endl;
    }
    return os;
}