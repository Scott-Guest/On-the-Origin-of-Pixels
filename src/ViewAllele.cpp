/*
 * ViewAllele.cpp
 * A view class to handle the Allele game mode.
 */

#include "ViewAllele.hpp"
#include <iomanip>
#include <sstream>

namespace GameOfLife {

ViewAllele::ViewAllele(unsigned int width, unsigned int height,
                       sf::RenderWindow &window, std::string output_file_name)
        : View(width, height, window, output_file_name), is_recording(false) {
    to_write.reserve(WRITE_DELTA);
} // ViewAllele()

// write any remaiing data to a file
ViewAllele::~ViewAllele() {
    if (output_file.is_open()) {
        output_file << to_write;
        to_write.clear();
        output_file.close();
    }
} // ~ViewAllele()

// update the colors of a single cell
void ViewAllele::update(unsigned int position, int color,
                        std::uint64_t generations_) {
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
    model_was_updated = true;
} // update()

// display the grid
void ViewAllele::display() {
    if (model_was_updated) {
        texture.update(&color_map[0]);
        model_was_updated = false;
    } // if

    std::stringstream stream;
    stream << "generations: " << std::to_string(generations)
           << ", recording: " << std::boolalpha << is_recording
           << ", genotype: " << geno;
    window.setTitle(std::string(stream.str()));
    window.draw(sprite);
} // display()

// write any recorded data to the output file
void ViewAllele::write_data(double dominant_freq, double recessive_freq,
                            double dominant_pheno, double recessive_pheno) {
    to_write += std::to_string(generations);
    to_write += ',';
    to_write += std::to_string(dominant_freq);
    to_write += ',';
    to_write += std::to_string(recessive_freq);
    to_write += ',';
    to_write += std::to_string(dominant_pheno);
    to_write += ',';
    to_write += std::to_string(recessive_pheno);
    to_write += '\n';
    size_t temp = to_write.size();
    if (temp >= WRITE_DELTA) {
        output_file << to_write;
        to_write.clear();
        to_write.reserve(WRITE_DELTA);
    }
} // write_date()

// toggle whether we are recording or not, creating a new file if needed
void ViewAllele::toggle_recording() {
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
        output_file.open(name, std::fstream::out);
        if (!output_file.is_open()) {
            assert(false);
        }
        to_write +=
                "Generation,Dominant Frequency,Recessive Frequency,Dominant "
                "Pheno. Frequency,Recessive Pheno. Frequency\n";
        ++extension_num;
    }
}

// update the title bar
void ViewAllele::update_title(std::string geno_) {
    geno = geno_;
    std::stringstream stream;
    stream << "generations: " << std::to_string(generations)
           << ", recording: " << std::boolalpha << is_recording
           << ", genotype: " << geno_;
    window.setTitle(std::string(stream.str()));
}

} // namespace GameOfLife
