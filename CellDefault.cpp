/*
 * CellDefault.cpp
 * A class representing a single cell in the defualt game mode.
 */

#include "CellDefault.hpp"
#include "ModelDefault.hpp"

namespace GameOfLife {

CellDefault::CellDefault(unsigned int row, unsigned int col, unsigned int color,
                         ModelDefault *model)
        : Cell(row, col, color), model(model) {} // CellAverage()

// count the number of living nieghbors adjacent to the cell
int CellDefault::count_neighbors() const {
    unsigned int height = model->height, width = model->width;
    int neighbors = 0;
    // iterate in a box around the current cell
    for (unsigned int row_current = row - 1; row_current <= row + 1;
         ++row_current) {
        for (unsigned int col_current = col - 1; col_current <= col + 1;
             ++col_current) {
            if (row_current < 0 || row_current >= height || col_current < 0 ||
                col_current >= width) {
                continue;
            }
            // don't count the cell as it's own neighbor
            if (row_current == row && col_current == col) {
                continue;
            }
            // count only living neighbors
            if (model->current_grid[width * row_current + col_current].color !=
                ModelDefault::IS_DEAD) {
                if (++neighbors > 3) {
                    return neighbors;
                }
            }
        }
    } // for
    return neighbors;
} // count_neighbors()

// update the cell for the next generation
void CellDefault::update() {
    CellDefault cell(*this);
    int neighbors = count_neighbors();

    // Any live cell with fewer than two live neighbors dies,
    // as if caused by under population.
    if (color == ModelDefault::IS_ALIVE && neighbors < 2) {
        cell.color = ModelDefault::IS_DEAD;
    } // if

    // Any live cell with two or three live neighbors lives on
    if (color == ModelDefault::IS_ALIVE && (neighbors == 2 || neighbors == 3)) {
        cell.color = ModelDefault::IS_ALIVE;
    } // if

    // Any live cell with more than three live neighbors dies,
    // as if by overpopulation.
    if (color == ModelDefault::IS_ALIVE && neighbors > 3) {
        cell.color = ModelDefault::IS_DEAD;
    } // if

    // Any dead cell with exactly three live neighbors becomes a live cell,
    // as if by reproduction.
    if (color == ModelDefault::IS_DEAD && neighbors == 3) {
        cell.color = ModelDefault::IS_ALIVE;
    } // if

    model->update_cell(row * model->width + col, cell);
} // update()

} // namespace GameOfLife
