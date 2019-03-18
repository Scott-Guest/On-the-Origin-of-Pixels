/*
 * ModelAllele.hpp
 * A model class to handle the Allele game mode.
 */

#ifndef ModelAllele_hpp
#define ModelAllele_hpp

#include "CellAllele.hpp"
#include "Model.hpp"
#include "ViewAllele.hpp"
#include <stdio.h>
#include <string>
#include <vector>

namespace GameOfLife {

class CellAllele;

class ModelAllele : public Model {
public:
    // construct a ModelAllele
    ModelAllele(unsigned int width, unsigned int height, double prob_alive,
                double dom_frequency, ViewAllele &view);
    // update the statistics recorded
    void update_stats();
    // update the board, swapping the current grid with the next grid
    void update() override;
    // reset the game board
    void reset() override;
    std::string get_geno(unsigned int x, unsigned int y);

private:
    // calculate the frequency of dominant alleles
    double calc_dominant_freq();
    // calculate the frequency of recessive alleles
    double calc_recessive_freq();
    // calculate the frequency of phenotypically dominant cells
    double calc_dominant_pheno();
    // calculate the frequency of phenotypically recessive cells
    double calc_recessive_pheno();
    double dom_frequency; // initial frequency of dominant alleles

    ViewAllele &view;

    // populate the grid with cells randomly
    void fill_grid();
    void update_cell(unsigned int position, const CellAllele &cell);

    // grid holding current board state
    std::vector<CellAllele> current_grid;
    // grid to write to for out of place calculation
    std::vector<CellAllele> next_grid;

    // number of recessive alleles (not cells!)
    unsigned int num_recessive;
    // number of dominant alleles (not cells!)
    unsigned int num_dominant;
    // frequency of phenotypically recessive cells
    unsigned int num_recessive_pheno;
    // frequency of phenotypically dominant cells
    unsigned int num_dominant_pheno;

    // dom/recessive colors
    static constexpr unsigned int DOMINANT = 0x0000FF;
    static constexpr unsigned int RECESSIVE = 0xFFFF00;

    friend class CellAllele;
};

} // namespace GameOfLife

#endif /* ModelAllele_hpp */
