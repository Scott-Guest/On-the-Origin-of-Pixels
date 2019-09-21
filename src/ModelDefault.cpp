/*
 * ModelDefault.hpp
 * A model class for the default game mode.
 */

#include "ModelDefault.hpp"
#include "CellDefault.hpp"

namespace GameOfLife {

ModelDefault::ModelDefault(unsigned int width, unsigned int height,
                           double prob_alive, ViewDefault &view)
        : Model(width, height, prob_alive), view(view) {
    reset();
} // ModelDefault()

// reset the board
void ModelDefault::reset() {
    generations = 0;
    current_grid.clear();
    current_grid.reserve(static_cast<std::size_t>(width * height));
    next_grid.clear();
    next_grid.reserve(static_cast<std::size_t>(width * height));
    for (unsigned int row = 0; row < height; ++row) {
        for (unsigned int col = 0; col < width; ++col) {
            current_grid.emplace_back(row, col,
                                      static_cast<unsigned int>(IS_DEAD), this);
            next_grid.emplace_back(row, col, static_cast<unsigned int>(IS_DEAD),
                                   this);
        }
    }
    fill_grid();
} // reset()

// fill the grid with random cells
void ModelDefault::fill_grid() {
    // chance of a cell forming
    std::uniform_real_distribution<double> cells_chance(0.0, 1.0);
    for (size_t pos = 0; pos < width * height; ++pos) {
        double prob = cells_chance(rng);
        if (prob <= prob_alive) {
            next_grid[pos].color = IS_ALIVE;
            view.update(static_cast<unsigned int>(pos), IS_ALIVE, 0u);
        } else {
            next_grid[pos].color = IS_DEAD;
            view.update(static_cast<unsigned int>(pos), IS_DEAD, 0u);
        } // else
    }     // for
} // fill_grid()

// update the game board to the next generation
void ModelDefault::update() {
    current_grid.swap(next_grid);
    ++generations;
    for (unsigned int row = 0; row < height; ++row) {
        for (unsigned int col = 0; col < width; ++col) {
            current_grid[row * width + col].update();
        } // for
    }     // for
} // update()

// update the cell at position with the contents of the input cell
void ModelDefault::update_cell(unsigned int position, const CellDefault &cell) {
    next_grid[position] = cell;
    view.update(position, static_cast<int>(cell.color), generations);
} // update_cell

} // namespace GameOfLife
