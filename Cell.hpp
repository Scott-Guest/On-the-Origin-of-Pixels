/*
 * Cell.hpp
 * A general class representing a cell in any game mode
 */

#ifndef Cell_hpp
#define Cell_hpp

#include <climits>
#include <functional>
#include <stdio.h>
#include <string>
#include <utility>

namespace GameOfLife {

class Cell {
public:
    Cell(unsigned int row, unsigned int col, unsigned int color)
            : row(row), col(col), color(color) {} // Cell()

    virtual void update() = 0;

    virtual ~Cell() {}

protected:
    unsigned int row, col, color;
};

} // namespace GameOfLife
#endif /* Cell_hpp */
