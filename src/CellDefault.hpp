/*
 * CellDefault.hpp
 * A class representing a single cell in the defualt game mode.
 */

#ifndef CellDefault_hpp
#define CellDefault_hpp

#include "Cell.hpp"
#include "ModelDefault.hpp"
#include <stdio.h>
namespace GameOfLife {

class ModelDefault;

class CellDefault : public Cell {
public:
    CellDefault(unsigned int row, unsigned int col, unsigned int color,
                ModelDefault *model);
    // update the cell for the next generation
    void update();

private:
    // count the number of living neighbors
    int count_neighbors() const;
    ModelDefault *model;
    friend class ModelDefault;
};

} // namespace GameOfLife
#endif /* CellDefault_hpp */
