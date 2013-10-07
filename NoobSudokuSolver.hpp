#ifndef NOOBSUDOKUSOLVER_HPP
#define NOOBSUDOKUSOLVER_HPP

#include <iostream>
#include <ostream>
#include <sstream>
#include <cstdint>
#include <cassert>

class NoobSudokuSolver
{
protected:
    struct Cell {
        std::bitset<9> flags;
        uint8_t value;
    } cells[9][9];
    std::bitset<9> cols[9][9], rows[9][9], grids[3][3][9];
public:
    size_t rec;
    NoobSudokuSolver(const char* str = nullptr);
    void reset();
    void set(uint8_t x, uint8_t y, uint8_t v);
    uint8_t decode(unsigned long v);
    void print(std::ostream& out);
};

#endif // NOOBSUDOKUSOLVER_HPP
