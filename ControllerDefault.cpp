//
//  ControllerDefault.cpp
//  GameOfLife
//
//  Created by Scott Guest on 3/17/19.
//  Copyright © 2019 Scott Guest. All rights reserved.
//

#include "ControllerDefault.hpp"

namespace GameOfLife {

ControllerDefault::ControllerDefault(unsigned int width, unsigned int height,
                                     double prob_alive,
                                     sf::RenderWindow &window,
                                     std::string output_file_name)
        : Controller(), view(width, height, window, output_file_name),
          model(width, height, prob_alive, view) {} // ControllerDefault()

// display the current board state
void ControllerDefault::display_view() {
    view.display();
}

// update the underlying board
void ControllerDefault::update_model() {
    if (!is_paused) {
        model.update();
    } // if
} // update_model()

// respond to key presses
void ControllerDefault::on_key_press(const sf::Event &event) {
    // toggle pausinbg
    if (event.key.code == sf::Keyboard::P) {
        is_paused = !is_paused;
    } // if

    // if we are paused, step through generations one by one
    if (is_paused && event.key.code == sf::Keyboard::U) {
        model.update();
    } // if

    // reset the game board
    if (event.key.code == sf::Keyboard::R) {
        model.reset();
    } // if
} // on_key_press()

} // namespace GameOfLife
