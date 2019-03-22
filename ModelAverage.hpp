/*
 * ModelAverage.hpp
 * A model class to handle the Average game mode.
 */

#ifndef ModelAverage_hpp
#define ModelAverage_hpp

#include "CellAverage.hpp"
#include "Model.hpp"
#include "ViewAverage.hpp"
#include <stdio.h>
#include <vector>

namespace GameOfLife {

class CellAverage;

class ModelAverage : public Model {
public:
    ModelAverage(unsigned int width, unsigned int height, double prob_alive,
                 bool sexual, double min_mutation, double max_mutation,
                 unsigned int ideal_color, ViewAverage &view);

    // update the baord for the next generation
    void update() override;
    // reset the game board
    void reset() override;
    // set the ideal color for determining fitness
    void set_ideal_color(unsigned int ideal_color_);
    // increase each channel of the ideal color by the given amounts
    void increase_ideal_color(int d_red, int d_green, int d_blue);

    // calculate the average fitness among all living cells
    double calc_average_fitness() const;
    // calculate the average mutation rate among all living cells
    double calc_average_mutation_rate() const;
    // calculate the standard deviation of fitness among living cells
    double calc_fitness_deviation() const;
    // recompute all statistics for the current board
    void update_stats();

private:
    ViewAverage &view;
    // fill the grid with random cells based on input probabilities
    void fill_grid();
    // update the cell at position to be the same as input cell
    void update_cell(unsigned int position, const CellAverage &cell);

    std::vector<CellAverage> current_grid;
    std::vector<CellAverage> next_grid;

    // ROYGBV
    int start_colors[6]{0xFF0000, 0xFF7F00, 0xFFFF00,
                        0x00FF00, 0x0000FF, 0x8B00FF};

    //    int start_colors[6]{0xFFFFFF, 0xFFFFFF, 0xFFFFFF,
    //                        0xFFFFFF, 0xFFFFFF, 0xFFFFFF};
    // equidistributed
    //    int start_colors[6]{0x2AAAAA, 0x555554, 0x7FFFFE, 0xAAAAA8, 0xD55552,
    //    0xFFFFFC};

    unsigned int ideal_red, orig_ideal_red;
    unsigned int ideal_green, orig_ideal_green;
    unsigned int ideal_blue, orig_ideal_blue;

    bool sexual;
    double min_mutation;
    double max_mutation;

    double total_fitness;
    double total_mutation_rate;
    friend class CellAverage;
};

} // namespace GameOfLife
#endif /* ModelAverage_hpp */
