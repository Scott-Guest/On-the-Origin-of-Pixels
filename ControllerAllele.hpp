/*
 * ControllerAllele.hpp
 * A controller class to handle the Allele game mode.
 */

#ifndef ControllerAllele_hpp
#define ControllerAllele_hpp

#include "Controller.hpp"
#include "ModelAllele.hpp"
#include "ViewAllele.hpp"
#include <stdio.h>

namespace GameOfLife {

class ControllerAllele : public Controller {
public:
    // construct an AlleleController
    ControllerAllele(unsigned int width, unsigned int height, double prob_alive,
                     double dom_frequency, sf::RenderWindow &window,
                     std::string output_file_name);
    // display the view of the current board
    void display_view() override;
    // update the underlying board
    void update_model() override;
    // handle key press events
    void on_key_press(const sf::Event &event) override;
    // update the titlebar to show the genotype of the cell currently under the
    // mouse
    void update_geno();

private:
    unsigned int width, height; // width and height of the board
    sf::RenderWindow &window;
    ViewAllele view;
    ModelAllele model;
};

} // namespace GameOfLife
#endif /* ControllerAllele_hpp */
