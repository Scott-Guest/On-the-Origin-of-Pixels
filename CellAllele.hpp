/*
 * CellAllele.cpp
 * A class representing a single cell in the Allele Game Mode
 */

#ifndef CellAllele_hpp
#define CellAllele_hpp

#include "Cell.hpp"
#include "ModelAllele.hpp"
#include <stdio.h>
#include <vector>

namespace GameOfLife {

class ModelAllele;

class CellAllele : public Cell {
public:
    // construct a Cell
    CellAllele(unsigned int row, unsigned int col, unsigned int color,
               std::uint64_t birth, ModelAllele *model);

    // update a single sell for the next generation
    void update();

    // update the color to the correct phenotype based on the current allele
    // pair
    void update_color();

private:
    u_int64_t generation_of_birth;
    unsigned int allele_pair[2];

    // get the next allele pair for this cell, choosing two parents based on
    // fitness
    std::pair<unsigned int, unsigned int>
    get_new_allele_pair(int &num_neighbors) const;

    // a pointer to the model holding this cell
    ModelAllele *model;
    friend class ModelAllele;
};

} // namespace GameOfLife

#endif /* CellAllele_hpp */
