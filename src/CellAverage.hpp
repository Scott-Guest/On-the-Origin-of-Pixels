/*
 * CellAverage.hpp
 * A class representing a single cell in the Average Game Mode
 */

#ifndef CellAverage_hpp
#define CellAverage_hpp

#include "Cell.hpp"
#include "ModelAverage.hpp"
#include <stdio.h>
#include <vector>

namespace GameOfLife {

class ModelAverage;

class CellAverage : public Cell {
public:
    CellAverage(unsigned int row, unsigned int col, unsigned int color,
                bool sexual, double mutation_rate, ModelAverage *model);
    // update the cell to the next generation
    void update();

private:
    // slightly change the color of a cell based on their mutation rate
    void mutate_color();
    // get Euclidean distance between color and ideal color
    double calc_fitness() const;

    std::pair<unsigned int, double>
    // count the number of living nieghbors, return new color and mutation rate
    // if between 2 and 3
    get_parents_average(int &num_neighbors) const;

    bool sexual;
    double mutation_rate;
    double fitness;
    ModelAverage *model;
    friend class ModelAverage;

    // max distance between any two colors (white and black)
    static constexpr double MAX_DISTANCE =
            441.67295593006370984949881708399745357;
};

} // namespace GameOfLife
#endif /* CellAverage_hpp */
