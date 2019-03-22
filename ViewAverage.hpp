/*
 * ViewAverage.hpp
 * A view class to handle the Allele game mode.
 */

#ifndef ViewAverage_hpp
#define ViewAverage_hpp

#include "View.hpp"
#include <stdio.h>
namespace GameOfLife {

class ViewAverage : public View {
public:
    ViewAverage(unsigned int width, unsigned int height,
                sf::RenderWindow &window, std::string output_file_name);
    // write any remaining data to file
    ~ViewAverage();
    // update the colors of a single cell
    void update(unsigned int position, int color, std::uint64_t generations_,
                unsigned int ideal_color_);
    // write a row of data to an output file
    void write_data(unsigned int num_alive, double average_mutation_rate,
                    double average_fitness, double fitness_deviation);
    // display the grid
    void display() override;
    // toggle recording on and off, writing to the output file
    void toggle_recording();

private:
    unsigned int ideal_color;
    std::string to_write;
    static constexpr unsigned int WRITE_DELTA = 1000;
    bool is_recording;
};

} // namespace GameOfLife
#endif /* ViewAverage_hpp */
