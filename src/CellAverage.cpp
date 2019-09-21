/*
 * CellAverage.cpp
 * A class representing a single cell in the Average Game Mode
 */

#include "CellAverage.hpp"
#include "ModelAverage.hpp"
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdlib>
namespace GameOfLife {

CellAverage::CellAverage(unsigned int row, unsigned int col, unsigned int color,
                         bool sexual, double mutation_rate, ModelAverage *model)
        : Cell(row, col, color), sexual{sexual},
          mutation_rate{mutation_rate}, model{model} {
    fitness = calc_fitness();
} // CellAverage()

// get Euc distance between color and ideal color
double CellAverage::calc_fitness() const {
    unsigned int red = (color >> 16) & 0xFF;
    unsigned int green = (color >> 8) & 0xFF;
    unsigned int blue = color & 0xFF;
    unsigned int ideal_red = model->ideal_red;
    unsigned int ideal_green = model->ideal_green;
    unsigned int ideal_blue = model->ideal_blue;

    unsigned int dred = red > ideal_red ? red - ideal_red : ideal_red - red;
    unsigned int dgreen =
            green > ideal_green ? green - ideal_green : ideal_green - green;
    unsigned int dblue =
            blue > ideal_blue ? blue - ideal_blue : ideal_blue - blue;
    double dist = std::pow(dred * dred + dgreen * dgreen + dblue * dblue, 0.5);
    return 100 * (MAX_DISTANCE - dist) / MAX_DISTANCE;
}

// count the number of living nieghbors, return new color and mutation rate if
// between 2 and 3
std::pair<unsigned int, double>
CellAverage::get_parents_average(int &num_neighbors) const {
    num_neighbors = 0;
    std::pair<unsigned int, unsigned int> parent_pos;
    double best_fitness = -DBL_MAX, second_best_fitness = -DBL_MAX;
    unsigned int height = model->height, width = model->width;
    for (unsigned int row_current = row - 1; row_current <= row + 1;
         ++row_current) {
        for (unsigned int col_current = col - 1; col_current <= col + 1;
             ++col_current) {
            if (row_current < 0 || row_current >= height || col_current < 0 ||
                col_current >= width) {
                continue;
            } // if

            if (row_current == row && col_current == col) {
                continue;
            } // if
            const CellAverage &cell =
                    model->current_grid[width * row_current + col_current];
            if (cell.color != ModelAverage::IS_DEAD) {
                ++num_neighbors;
                double fitness = cell.fitness;
                if (fitness >= best_fitness) {
                    second_best_fitness = best_fitness;
                    parent_pos.second = parent_pos.first;
                    best_fitness = fitness;
                    parent_pos.first = row_current * width + col_current;
                } else if (fitness >= second_best_fitness) {
                    second_best_fitness = fitness;
                    parent_pos.second = row_current * width + col_current;
                } // if
            }     // if
        }         // for
    }             // for
    std::pair<unsigned int, double> result;
    if (num_neighbors != 2 && num_neighbors != 3) {
        result.first = 0;
        result.second = 0;
        return result;
    }
    CellAverage &par1 = model->current_grid[parent_pos.first];
    // if asexual, just copy the parent
    if (!sexual) {
        result.first = par1.color;
        result.second = par1.mutation_rate;
        return result;
    }
    // otherwise, get the average of the parents
    CellAverage &par2 = model->current_grid[parent_pos.second];
    unsigned int color_1 = par1.color;
    unsigned int color_2 = par2.color;
    unsigned int red =
            (((color_1 >> 16) & 0xFF) + ((color_2 >> 16) & 0xFF)) / 2;
    unsigned int green =
            (((color_1 >> 8) & 0xFF) + ((color_2 >> 8) & 0xFF)) / 2;
    unsigned int blue = ((color_1 & 0xFF) + (color_2 & 0xFF)) / 2;
    result.first = ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff);
    result.second = par1.mutation_rate / 2 + par2.mutation_rate / 2;
    return result;
} // get_parents_average()

// slightly change the color of a cell based on their mutation rate
void CellAverage::mutate_color() {
    if (mutation_rate == 0) return;
    unsigned int rgb[3]{((color >> 16) & 0xFF), ((color >> 8) & 0xFF),
                        (color & 0xFF)};
    for (int i = 0; i < 3; ++i) {
        unsigned int channel = rgb[i];
        std::uniform_int_distribution<int> sign(0, 1);
        // if sign gives 1, then increase the color channel if possible
        if (sign(model->rng) && channel < 0xFF) {
            double n = static_cast<double>(0xFF - channel);
            double len = (1 / mutation_rate - 1) * (n - 1);
            std::uniform_real_distribution<double> mut(1, n + len);
            unsigned int delta = static_cast<unsigned int>(
                    mut(model->rng)); // change in color
            delta = delta > n ? 0
                              : static_cast<unsigned int>(
                                        std::floor(n / delta + 1) - 1);
            rgb[i] += static_cast<unsigned int>(delta);
        } else if (channel > 0x0) {
            // otherwise decrease the color channel if possible
            double n = static_cast<double>(channel);
            double len = (1 / mutation_rate - 1) * (n - 1);
            std::uniform_real_distribution<double> mut(1, n + len);
            double delta = mut(model->rng); // change in color
            delta = delta > n ? 0 : std::floor(n / delta + 1) - 1;
            rgb[i] -= static_cast<unsigned int>(delta);
        } // if
    }     // for
    color = ((rgb[0] & 0xFF) << 16) + ((rgb[1] & 0xFF) << 8) + (rgb[2] & 0xFF);
} // mutate_color()

// update the cell to the next generation
void CellAverage::update() {
    CellAverage cell(*this);
    int neighbors;
    std::pair<unsigned int, double> avg = get_parents_average(neighbors);

    // Any live cell with fewer than two live neighbors or more than
    // 3 dies, as if caused by under/over population.
    if (cell.color != ModelAverage::IS_DEAD &&
        (neighbors < 2 || neighbors > 3)) {
        cell.color = ModelAverage::IS_DEAD;
    } else if (cell.color == ModelAverage::IS_DEAD && neighbors == 3) {
        // Any dead cell with exactly three live neighbors becomes a live cell,
        // as if by reproduction.
        cell.color = avg.first;
        cell.mutation_rate = avg.second;
        cell.mutate_color();
        cell.fitness = cell.calc_fitness();
    } else if (cell.color == ModelAverage::IS_DEAD && neighbors == 2) {
        std::uniform_real_distribution<double> cells_chance(0.0, 1.0);
        double prob = cells_chance(model->rng);
        if (prob <= 0.1) {
            cell.color = avg.first;
            cell.mutation_rate = avg.second;
            cell.mutate_color();
            cell.fitness = cell.calc_fitness();
        }
    } // if
    model->update_cell(row * model->width + col, cell);
} // update()

} // namespace GameOfLife
