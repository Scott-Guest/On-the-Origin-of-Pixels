/*
 * ModelAverage.cpp
 * A model class to handle the Average game mode.
 */
#include "ModelAverage.hpp"
#include "CellAverage.hpp"

namespace GameOfLife {

ModelAverage::ModelAverage(unsigned int width, unsigned int height,
                           double prob_alive, bool sexual, double min_mutation,
                           double max_mutation, unsigned int ideal_color,
                           ViewAverage &view)
        : Model(width, height, prob_alive), view(view), sexual(sexual),
          min_mutation(min_mutation), max_mutation(max_mutation),
          total_fitness(0), total_mutation_rate(0) {
    set_ideal_color(ideal_color);
    // store the original ideal color channels
    orig_ideal_red = ideal_red;
    orig_ideal_green = ideal_green;
    orig_ideal_blue = ideal_blue;
    reset();
} // ModelAverage()

// reset the game board
void ModelAverage::reset() {
    generations = 0;
    num_alive = 0;
    total_fitness = 0;
    total_mutation_rate = 0;
    ideal_red = orig_ideal_red;
    ideal_green = orig_ideal_green;
    ideal_blue = orig_ideal_blue;
    current_grid.clear();
    current_grid.reserve(static_cast<std::size_t>(width * height));
    next_grid.clear();
    next_grid.reserve(static_cast<std::size_t>(width * height));
    for (unsigned int row = 0; row < height; ++row) {
        for (unsigned int col = 0; col < width; ++col) {
            std::uniform_real_distribution<double> mut(min_mutation,
                                                       max_mutation);
            current_grid.emplace_back(row, col, 0, sexual, mut(rng), this);
            next_grid.emplace_back(row, col, 0, sexual, mut(rng), this);
        }
    }
    fill_grid();
} // reset()

// fill the grid with random cells based on input probabilities
void ModelAverage::fill_grid() {
    // random color selector
    std::uniform_int_distribution<int> color_gen(0, 5);
    // chance of a cell forming
    std::uniform_real_distribution<double> cells_chance(0.0, 1.0);
    for (size_t pos = 0; pos < width * height; ++pos) {
        double prob = cells_chance(rng);
        unsigned int ideal_color = ((ideal_red & 0xFF) << 16) +
                                   ((ideal_green & 0xFF) << 8) +
                                   (ideal_blue & 0xFF);
        if (prob <= prob_alive) {
            next_grid[pos].color =
                    static_cast<unsigned int>(start_colors[color_gen(rng)]);
            double fitness = next_grid[pos].calc_fitness();
            next_grid[pos].fitness = fitness;
            total_mutation_rate += next_grid[pos].mutation_rate;
            view.update(static_cast<unsigned int>(pos),
                        static_cast<int>(next_grid[pos].color), 0u,
                        ideal_color);
        } else {
            next_grid[pos].color = IS_DEAD;
            view.update(static_cast<unsigned int>(pos), IS_DEAD, 0u,
                        ideal_color);
        } // else
    }     // for
    update_stats();
} // fill_grid()

// update the baord for the next generation
void ModelAverage::update() {
    current_grid.swap(next_grid);
    ++generations;
    for (unsigned int i = 0; i < width * height; ++i) {
        current_grid[i].update();
    } // for
    update_stats();
    // write data to file
    if (is_recording) {
        view.write_data(num_alive, calc_average_mutation_rate(),
                        calc_average_fitness(), calc_fitness_deviation());
    }
} // update()

// update the cell at position to be the same as input cell
void ModelAverage::update_cell(unsigned int position, const CellAverage &cell) {
    next_grid[position] = cell;
    unsigned int ideal_color = ((ideal_red & 0xFF) << 16) +
                               ((ideal_green & 0xFF) << 8) +
                               (ideal_blue & 0xFF);
    view.update(position, static_cast<int>(cell.color), generations,
                ideal_color);
} // update_cell

// set the ideal color for determining fitness
void ModelAverage::set_ideal_color(unsigned int ideal_color) {
    ideal_red = (ideal_color >> 16) & 0xFF;
    ideal_green = (ideal_color >> 8) & 0xFF;
    ideal_blue = ideal_color & 0xFF;
} // set_ideal_color()

// increase each channel of the ideal color by the given amounts
void ModelAverage::increase_ideal_color(int d_red, int d_green, int d_blue) {
    if ((d_red < 0 && ideal_red >= static_cast<unsigned int>(-d_red)) ||
        (d_red > 0 && 0xFF - ideal_red >= static_cast<unsigned int>(d_red))) {
        ideal_red += static_cast<unsigned int>(d_red);
    }
    if ((d_green < 0 && ideal_green >= static_cast<unsigned int>(-d_green)) ||
        (d_green > 0 &&
         0xFF - ideal_green >= static_cast<unsigned int>(d_green))) {
        ideal_green += static_cast<unsigned int>(d_green);
    }
    if ((d_blue < 0 && ideal_blue >= static_cast<unsigned int>(-d_blue)) ||
        (d_blue > 0 &&
         0xFF - ideal_blue >= static_cast<unsigned int>(d_blue))) {
        ideal_blue += static_cast<unsigned int>(d_blue);
    }
} // increase_ideal_color()

// calculate the average fitness among all living cells
double ModelAverage::calc_average_fitness() const {
    return total_fitness / static_cast<double>(num_alive);
}

// calculate the average mutation rate among all living cells
double ModelAverage::calc_average_mutation_rate() const {
    return total_mutation_rate / static_cast<double>(num_alive);
}

// calculate the standard deviation of fitness among living cells
double ModelAverage::calc_fitness_deviation() const {
    double average = calc_average_fitness();
    double sum = 0;
    for (unsigned int i = 0; i < width * height; ++i) {
        if (current_grid[i].color != ModelAverage::IS_DEAD) {
            double temp = current_grid[i].fitness - average;
            sum += temp * temp;
        }
    }
    sum /= static_cast<double>(num_alive);
    return std::pow(sum, 0.5);
}

// recompute all statistics for the current board
void ModelAverage::update_stats() {
    total_fitness = 0;
    total_mutation_rate = 0;
    num_alive = 0;
    for (unsigned int i = 0; i < width * height; ++i) {
        if (next_grid[i].color != ModelAverage::IS_DEAD) {
            next_grid[i].fitness = next_grid[i].calc_fitness();
            total_fitness += next_grid[i].fitness;
            total_mutation_rate += next_grid[i].mutation_rate;
            ++num_alive;
        } else {
            next_grid[i].fitness = 0;
        } // if/else
    }     // for
} // update_stats()

} // namespace GameOfLife
