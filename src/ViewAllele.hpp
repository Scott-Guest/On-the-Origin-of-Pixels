/*
 * ViewAllele.hpp
 * A view class to handle the Allele game mode.
 */

#ifndef ViewAllele_hpp
#define ViewAllele_hpp

#include "View.hpp"
#include <stdio.h>

namespace GameOfLife {

class ViewAllele : public View {
public:
    ViewAllele(unsigned int width, unsigned int height,
               sf::RenderWindow &window, std::string output_file_name);

    // write any remaining data to a file
    ~ViewAllele();

    // update the colors of a single cell
    void update(unsigned int position, int color, std::uint64_t generations_);

    // write any recorded data to the output file
    void write_data(double dominant_freq, double recessive_freq,
                    double dominant_pheno, double recessive_phenos);
    // display the grid
    void display() override;

    // toggle whether we are recording or not, creating a new file if needed
    void toggle_recording();

    // update the title bar
    void update_title(std::string geno);

private:
    std::string geno;
    std::string to_write;
    static constexpr unsigned int WRITE_DELTA = 1000;
    bool is_recording;
};

} // namespace GameOfLife
#endif /* ViewAllele_hpp */
