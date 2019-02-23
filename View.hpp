/*
 * View.hpp
 * A general class representing a view for any game mode
 */

#ifndef View_hpp
#define View_hpp

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>
namespace GameOfLife {

class View {
public:
    // cosntruct a view and set up the window
    View(unsigned int width, unsigned int height, sf::RenderWindow &window,
         std::string output_file_name);
    // close the output files
    ~View();

    // display the grid
    virtual void display() = 0;

protected:
    static constexpr unsigned int R_POS = 0;
    static constexpr unsigned int G_POS = 1;
    static constexpr unsigned int B_POS = 2;
    static constexpr unsigned int A_POS = 3;
    static constexpr unsigned int RGBA_BLOCK = 4;
    static constexpr std::uint_fast8_t MAX_RGB = 255;
    static constexpr std::uint_fast8_t FULL_ALPHA = 255;
    std::vector<std::uint_fast8_t> color_map;

    sf::Texture texture;
    sf::Sprite sprite;
    float xScale, yScale;

    sf::RenderWindow &window;
    std::uint64_t generations;
    bool model_was_updated;

    std::string output_file_name;
    std::ofstream output_file;
    int extension_num;
};

} // namespace GameOfLife
#endif /* View_hpp */
