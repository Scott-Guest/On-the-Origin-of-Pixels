/*
 * ModelAllele.cpp
 * A model class to handle the Allele game mode.
 */

#include "ModelAllele.hpp"
#include "CellAllele.hpp"

namespace GameOfLife {

// cosntruct a ModelAllele
ModelAllele::ModelAllele(unsigned int width, unsigned int height,
                         double prob_alive, double dom_frequency,
                         ViewAllele &view)
        : Model(width, height, prob_alive), dom_frequency(dom_frequency),
          view(view), num_recessive(0), num_dominant(0), num_recessive_pheno(0),
          num_dominant_pheno(0) {
    reset();
} // ModelAllele()

// reset the game board
void ModelAllele::reset() {
    generations = 0;
    num_alive = 0;
    current_grid.clear();
    current_grid.reserve(static_cast<std::size_t>(width * height));
    next_grid.clear();
    next_grid.reserve(static_cast<std::size_t>(width * height));
    for (unsigned int row = 0; row < height; ++row) {
        for (unsigned int col = 0; col < width; ++col) {
            current_grid.emplace_back(row, col, 0, 0, this);
            next_grid.emplace_back(row, col, 0, 0, this);
        }
    }
    fill_grid();
} // reset()

// populate the grid with cells randomly
void ModelAllele::fill_grid() {
    // chance of a cell forming / having allel
    std::uniform_real_distribution<double> cells_chance(0.0, 1.0);
    for (size_t pos = 0; pos < width * height; ++pos) {
        double prob = cells_chance(rng);
        // determine if the cell should be alive or dead
        if (prob <= prob_alive) {
            double allele_1_prob = cells_chance(rng);
            double allele_2_prob = cells_chance(rng);
            // randomly set the allele pair
            next_grid[pos].allele_pair[0] =
                    allele_1_prob <= dom_frequency ? DOMINANT : RECESSIVE;
            next_grid[pos].allele_pair[1] =
                    allele_2_prob <= dom_frequency ? DOMINANT : RECESSIVE;
            next_grid[pos].update_color();
            // update the view
            view.update(static_cast<unsigned int>(pos),
                        static_cast<int>(next_grid[pos].color), 0u);
        } else {
            next_grid[pos].color = IS_DEAD;
            view.update(static_cast<unsigned int>(pos), IS_DEAD, 0u);
        } // else
    }     // for
    // update all the board statistics
    update_stats();
} // fill_grid()

// calculate the frequency of dominant alleles
double ModelAllele::calc_dominant_freq() {
    return static_cast<double>(num_dominant) /
           static_cast<double>(2 * num_alive);
} // calc_dominant_freq()

// calculate the frequency of recessive alleles
double ModelAllele::calc_recessive_freq() {
    return static_cast<double>(num_recessive) /
           static_cast<double>(2 * num_alive);
} // calc_recessive_freq()

// calculate the frequency of phenotypically dominant cells
double ModelAllele::calc_dominant_pheno() {
    return static_cast<double>(num_dominant_pheno) /
           static_cast<double>(num_alive);
} // calc_dominant_pheno()

// calculate the frequency of phenotypically recessive cells
double ModelAllele::calc_recessive_pheno() {
    return static_cast<double>(num_recessive_pheno) /
           static_cast<double>(num_alive);
} // calc_recessive_pheno()

// update the board, swapping the current grid with the next grid
void ModelAllele::update() {
    current_grid.swap(next_grid);
    ++generations;
    for (unsigned int i = 0; i < width * height; ++i) {
        current_grid[i].update();
    } // for
    update_stats();
    if (is_recording) {
        view.write_data(calc_dominant_freq(), calc_recessive_freq(),
                        calc_dominant_pheno(), calc_recessive_pheno());
    }
} // update()

// update a cell at a given position to that of cell
void ModelAllele::update_cell(unsigned int position, const CellAllele &cell) {
    next_grid[position] = cell;
    view.update(position, static_cast<int>(cell.color), generations);
} // update_cell

// recalculate all game statistics
void ModelAllele::update_stats() {
    num_alive = 0;
    num_dominant = 0;
    num_recessive = 0;
    num_dominant_pheno = 0;
    num_recessive_pheno = 0;
    for (unsigned int i = 0; i < width * height; ++i) {
        if (next_grid[i].color != ModelAllele::IS_DEAD) {
            ++num_alive;
            next_grid[i].update_color();
            if (next_grid[i].color == ModelAllele::DOMINANT) {
                ++num_dominant_pheno;
                // if either is recessive, then we have one recessive and one
                // dominant
                if (next_grid[i].allele_pair[0] == ModelAllele::RECESSIVE ||
                    next_grid[i].allele_pair[1] == ModelAllele::RECESSIVE) {
                    ++num_recessive;
                    ++num_dominant;
                } else {
                    num_dominant += 2; // 2 dominant alleles present
                }
            } else if (next_grid[i].color == ModelAllele::RECESSIVE) {
                ++num_recessive_pheno;
                num_recessive += 2; // 2 recessive alleles present
            }
        }
    }
} // update_stats()

// return the genome of the cell at position (x, y)
std::string ModelAllele::get_geno(unsigned int x, unsigned int y) {
    if (y * width + x >= width * height) {
        return std::string("ERROR");
    }

    CellAllele &curr = next_grid[y * width + x];
    if (curr.color == IS_DEAD) {
        return std::string("DEAD");
    }
    std::string geno;
    char first = curr.allele_pair[0] == DOMINANT ? 'A' : 'a';
    char second = curr.allele_pair[1] == DOMINANT ? 'A' : 'a';
    geno.push_back(first);
    geno.push_back(second);
    return geno;
} // get_geno

} // namespace GameOfLife
