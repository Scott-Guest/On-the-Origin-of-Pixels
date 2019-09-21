/*
 * ControllerAverage.hpp
 * A controller class to handle the Average game mode.
 */

#ifndef ControllerAverage_hpp
#define ControllerAverage_hpp

#include "Controller.hpp"
#include "ModelAverage.hpp"
#include "ViewAverage.hpp"
#include <stdio.h>

namespace GameOfLife {

class ControllerAverage : public Controller {
public:
    ControllerAverage(unsigned int width, unsigned int height,
                      double prob_alive, bool sexual, double min_mutation,
                      double max_mutation, unsigned int ideal_color,
                      sf::RenderWindow &window, std::string output_file_name);
    // display the view of the current board
    void display_view() override;
    // update the game board if we aren't paused
    void update_model() override;
    // respond to key presses
    void on_key_press(const sf::Event &event) override;
    // set the ideal color used for determining fitness
    void set_ideal_color(int ideal_color);

private:
    ViewAverage view;
    ModelAverage model;
    static constexpr double COLOR_DELTA = 0x1;
    double red_change, green_change, blue_change;
};

} // namespace GameOfLife

#endif /* ControllerAverage_hpp */
