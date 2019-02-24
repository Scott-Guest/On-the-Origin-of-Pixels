/*
 * ViewDefault.cpp
 * A view class to handle the default game mode.
 */
#include "ViewDefault.hpp"

namespace GameOfLife {

ViewDefault::ViewDefault(unsigned int width, unsigned int height,
                         sf::RenderWindow &window, std::string output_file_name)
        : View(width, height, window, output_file_name) {} // ViewDefault()

// update the colors of a single cell
void ViewDefault::update(unsigned int position, int color,
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
void ViewDefault::display() {
    if (model_was_updated) {
        texture.update(&color_map[0]);
        model_was_updated = false;
    } // if

    window.setTitle(std::to_string(generations));
    window.draw(sprite);
} // display()

} // namespace GameOfLife
