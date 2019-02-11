//
// Created by mpark on 2/10/19.
//
#ifndef SUDOKU_SUDOKUSOLVER_H
#define SUDOKU_SUDOKUSOLVER_H
#include <vector>
#include <iostream>
#include <array>

/**
 * Uses Backtracking algorithm to solve a Sudoku problem
 */
class SudokuSolver {
private:
    /**
     * Represent each of the 9 regions on the Sudoku board in which all numbers have to be unique
     */
    struct Region {
        std::size_t lowX;
        std::size_t highX;
        std::size_t lowY;
        std::size_t highY;

        /**
         * Region Constructor
         * @param lx std::size_t low x-coordinate boundary
         * @param hx std::size_t high x-coordinate boundary
         * @param ly std::size_t low y-coordinate boundary
         * @param hy std::size_t high y-coordinate boundary
         */
        Region(std::size_t lx, std::size_t hx, std::size_t ly, std::size_t hy);

        /**
         * Given x and y coordinates, determine if the coordinates are in this region
         * @param x std::size_t x-coordinate
         * @param y std::size_t y-coordinate
         * @return bool true if this region contains the given coordinates
         */
        bool contains(std::size_t x, std::size_t y) const;
    };

    /**
     * The size of a Sudoku board is always 9 X 9
     */
    const static std::size_t BOARD_SIZE;

    /**
     * Holds possible range of numbers to write on the Sudoku board
     */
    const static std::array<int, 9> possibleValues;

    /**
     * Holds 9 regions of the board in which all numbers have to be unique
     */
    std::vector<Region> regions;

    /**
     * Entire Sudoku board that holds all 9 regions
     */
    std::vector<std::vector<int>> board;

    /**
     * Has this Sudoku problem been solved?
     */
    bool solved;

    /**
     * Has the Sudoku board been initialized?
     */
    bool initialized;

public:
    /**
     * Default Constructor
     */
    SudokuSolver();

    /**
     * Public function to start solving this Sudoku problem
     * @pre initialized is true
     * @post solved is set to true
     */
    void solve();

    /**
     * Read from the input stream and initialize the Sudoku board.
     * @param is std::istream & input stream
     * @param sudokuSolver SudokuSolver & SudokuSolver object
     * @post initialized is set to true
     * @post solved is set to false
     * @return std::istream & input stream
     */
    friend std::istream & operator>>(std::istream & is, SudokuSolver & sudokuSolver);

    /**
     * Print the solved Sudoku board
     * @param os
     * @param sudokuSolver
     * @pre initialized is true
     * @pre solved is true
     * @return
     */
    friend std::ostream & operator<<(std::ostream & os, const SudokuSolver & sudokuSolver);

private:
    /**
     * Recursive function to solve the Sudoku puzzle using backtracking
     * @param int Only exists to differentiate 2 functions with the same name
     * @return bool true if solved
     */
    bool solve(int);

    /**
     * Check if the value at the given coordinates is valid
     * @param x std::size_t x-coordinate
     * @param y std::size_t y-coordinate
     * @param value int value
     * @return bool true if the value is valid
     */
    bool isValid(std::size_t x, std::size_t y, int value) const;

    /**
     * Determine if the Sudoku board is complete.
     * If the board is complete, set x and y to -1.
     * If the board is not complete, set the first unsolved x and y coordinates.
     * @param x std::size_t & x-coordinate
     * @param y std::size_t & y-coordinate
     * @return bool true if the Sudoku board is complete
     */
    bool isComplete(std::size_t & x, std::size_t & y) const;

    /**
     * Determine which region the given coordinates are located
     * @param x std::size_t x-coordinate
     * @param y std::size_t y-coordinate
     * @return Region & Region object
     */
    const Region & getRegion(std::size_t x, std::size_t y) const;
};


#endif //SUDOKU_SUDOKUSOLVER_H
