/*
 * ViewAverage.cpp
 * A view class to handle the Allele game mode.
 */

#include "ViewAverage.hpp"
#include <iomanip>
#include <sstream>

namespace GameOfLife {

ViewAverage::ViewAverage(unsigned int width, unsigned int height,
                         sf::RenderWindow &window, std::string output_file_name)
        : View(width, height, window, output_file_name), ideal_color(0),
          is_recording(false) {
    to_write.reserve(WRITE_DELTA);
} // ViewAverage()

// write any remaining data to files
ViewAverage::~ViewAverage() {
    if (output_file.is_open()) {
        output_file << to_write;
        to_write.clear();
        output_file.close();
    }
} // ~ViewAverage()

// update the colors of a single cell
void ViewAverage::update(unsigned int position, int color,
                         std::uint64_t generations_,
                         unsigned int ideal_color_) {
    // get RGB data from hex value
    std::uint_fast8_t red = ((color >> 16) & 0xFF);
    std::uint_fast8_t green = ((color >> 8) & 0xFF);
    std::uint_fast8_t blue = color & 0xFF;

    // multi position by length of block to get start of pos block
    position *= RGBA_BLOCK;
    color_map[position + R_POS] = red;
    color_map[position + G_POS] = green;
    color_map[position + B_POS] = blue;
    color_map[position + A_POS] = FULL_ALPHA;

    generations = generations_;
    ideal_color = ideal_color_;
    model_was_updated = true;
} // update()

// display the grid
void ViewAverage::display() {
    if (model_was_updated) {
        texture.update(&color_map[0]);
        model_was_updated = false;
    } // if

    std::stringstream stream;
    stream << "generations: " << std::to_string(generations) << ", "
           << "ideal color: 0x" << std::uppercase << std::hex
           << static_cast<uintptr_t>(ideal_color)
           << ", recording: " << std::boolalpha << is_recording;
    window.setTitle(std::string(stream.str()));
    window.draw(sprite);
} // display()

// write a row of data to an output file
void ViewAverage::write_data(unsigned int num_alive_,
                             double average_mutation_rate,
                             double average_fitness, double fitness_deviation) {
    to_write += std::to_string(generations);
    to_write += ',';
    to_write += std::to_string(ideal_color);
    to_write += ',';
    to_write += std::to_string(num_alive_);
    to_write += ',';
    to_write += std::to_string(average_mutation_rate);
    to_write += ',';
    to_write += std::to_string(average_fitness);
    to_write += ',';
    to_write += std::to_string(fitness_deviation);
    to_write += '\n';
    size_t temp = to_write.size();
    if (temp >= WRITE_DELTA) {
        output_file << to_write;
        to_write.clear();
        to_write.reserve(WRITE_DELTA);
    }
} // write_date()

// toggle recording on and off, writing to the output file
void ViewAverage::toggle_recording() {
    if (output_file.is_open()) {
        output_file << to_write;
        to_write.clear();
    }
    is_recording = !is_recording;
    if (is_recording) {
        output_file.close();
        std::string name = output_file_name;
        name += '_';
        name += std::to_string(extension_num);
        name += ".csv";
        std::cout << name << std::endl;
        output_file.open(name, std::fstream::out);
        if (!output_file.is_open()) {
            assert(false);
        }
        to_write +=
                "Generation,Ideal Color,Living,Mutation Rate,Fitness,Fitness "
                "Deviation\n";
        ++extension_num;
    }
}

} // namespace GameOfLife
