/*
 * View.cpp
 * A general class representing a view for any game mode
 */

#include "View.hpp"
#include <cassert>
namespace GameOfLife {

// cosntruct a view and set up the window
View::View(unsigned int width, unsigned int height, sf::RenderWindow &window,
           std::string output_file_name_)
        : color_map(), texture(), sprite(), window(window),
          output_file_name(""), extension_num(0) {
    color_map.resize(width * height * RGBA_BLOCK, 255);

    // set xScale and yScale for texture vs window
    sf::Vector2u windowSize = window.getSize();
    xScale = static_cast<float>(windowSize.x) / width;
    yScale = static_cast<float>(windowSize.y) / height;
    sprite.setScale(xScale, yScale);

    // set all RGBA values to black and opaque
    for (unsigned int i = 0; i < width * height * RGBA_BLOCK; i += RGBA_BLOCK) {
        color_map[i + A_POS] = FULL_ALPHA;
    } // for

    texture.create(width, height);
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);

    output_file_name += output_file_name_;
} // View()

// close the output file
View::~View() {
    output_file.close();
}

} // namespace GameOfLife
