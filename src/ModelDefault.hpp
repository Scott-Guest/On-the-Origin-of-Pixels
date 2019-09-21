/*
 * ModelDefault.hpp
 * A model class for the default game mode.
 */

#ifndef ModelDefault_hpp
#define ModelDefault_hpp

#include "CellDefault.hpp"
#include "Model.hpp"
#include "ViewDefault.hpp"
#include <stdio.h>

namespace GameOfLife {

class CellDefault;

class ModelDefault : public Model {
public:
    ModelDefault(unsigned int width, unsigned int height, double prob_alive,
                 ViewDefault &view);
    // update the game board to the next generation
    void update() override;
    // reset the boards
    void reset() override;

private:
    ViewDefault &view;
    // fill the grid with random cells
    void fill_grid();
    // update the cell at position with the contents of the input cell
    void update_cell(unsigned int position, const CellDefault &cell);

    std::vector<CellDefault> current_grid;
    std::vector<CellDefault> next_grid;

    friend class CellDefault;
};

} // namespace GameOfLife
#endif /* ModelDefault_hpp */
