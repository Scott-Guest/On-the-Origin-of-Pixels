//
//  CellAllele.cpp
//  GameOfLife
//
//  Created by Scott Guest on 3/30/19.
//  Copyright © 2019 Scott Guest. All rights reserved.
//

#include "CellAllele.hpp"
#include "ModelAllele.hpp"
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdlib>

namespace GameOfLife {

CellAllele::CellAllele(unsigned int row, unsigned int col, unsigned int color,
                       std::uint64_t birth, ModelAllele *model)
        : Cell(row, col, color), generation_of_birth{birth}, model{model} {
} // CellAllele()

// Count the number of living nieghbors adjacent to a cell. Return a new allele
// pair from parents in the model if there are between 2 and 3 neighbors.
std::pair<unsigned int, unsigned int>
CellAllele::get_new_allele_pair(int &num_neighbors) const {
    num_neighbors = 0;
    unsigned int parent_pos[2];

    unsigned int height = model->height, width = model->width;
    // iterate in a box around the current pixel
    for (unsigned int row_current = row - 1; row_current <= row + 1;
         ++row_current) {
        for (unsigned int col_current = col - 1; col_current <= col + 1;
             ++col_current) {
            // check that we are in the bounds of the board
            if (row_current < 0 || row_current >= height || col_current < 0 ||
                col_current >= width) {
                continue;
            } // if
            // avoid counting the cell itself
            if (row_current == row && col_current == col) {
                continue;
            } // if
            const CellAllele &cell =
                    model->current_grid[width * row_current + col_current];
            if (cell.color != ModelAllele::IS_DEAD) {
                if (num_neighbors < 2) {
                    parent_pos[num_neighbors] =
                            width * row_current + col_current;
                }
                ++num_neighbors;
            } // if
        }     // for
    }         // for
    std::pair<unsigned int, unsigned int> result;
    if (num_neighbors != 2 && num_neighbors != 3) {
        result.first = 0;
        result.second = 0;
        return result;
    }

    // randomly choose an allele from each parrent
    CellAllele &par1 = model->current_grid[parent_pos[0]];
    CellAllele &par2 = model->current_grid[parent_pos[1]];
    std::uniform_int_distribution<int> choose(0, 1);
    result.first = par1.allele_pair[choose(model->rng)];
    result.second = par2.allele_pair[choose(model->rng)];
    return result;
} // get_parents_average()

// update the color based on the current allel pair
void CellAllele::update_color() {
    if (allele_pair[0] == model->DOMINANT ||
        allele_pair[1] == model->DOMINANT) {
        color = model->DOMINANT;
    } else if (allele_pair[0] == model->RECESSIVE &&
               allele_pair[1] == model->RECESSIVE) {
        color = model->RECESSIVE;
    } else {
        assert(false);
    }
} // update_color()

// update the cell for the next generation
void CellAllele::update() {
    CellAllele cell(*this);
    int neighbors;
    std::pair<unsigned int, unsigned int> alleles =
            get_new_allele_pair(neighbors);

    // Any live cell with fewer than two live neighbors or more than
    // 3 dies, as if caused by under/over population.
    if (cell.color != ModelAllele::IS_DEAD &&
        (neighbors < 2 || neighbors > 3)) {
        cell.color = ModelAllele::IS_DEAD;
    } else if (cell.color == ModelAllele::IS_DEAD && neighbors == 3) {
        // Any dead cell with exactly three live neighbors becomes a live cell,
        // as if by reproduction.
        cell.allele_pair[0] = alleles.first;
        cell.allele_pair[1] = alleles.second;
        cell.update_color();
        cell.generation_of_birth = model->generations;
    } else if (cell.color == ModelAllele::IS_DEAD && neighbors == 2) {
        std::uniform_real_distribution<double> cells_chance(0.0, 1.0);
        double prob = cells_chance(model->rng);
        if (prob <= 0.1) {
            cell.allele_pair[0] = alleles.first;
            cell.allele_pair[1] = alleles.second;
            cell.update_color();
            cell.generation_of_birth = model->generations;
        }
    } // if
    model->update_cell(row * model->width + col, cell);
} // update()

} // namespace GameOfLife
