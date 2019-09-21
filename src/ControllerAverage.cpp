/*
 * ControllerAverage.cpp
 * A controller class to handle the Average game mode.
 */

#include "ControllerAverage.hpp"

namespace GameOfLife {

// construct a controller for the average game mode
ControllerAverage::ControllerAverage(unsigned int width, unsigned int height,
                                     double prob_alive, bool sexual,
                                     double min_mutation, double max_mutation,
                                     unsigned int ideal_color,
                                     sf::RenderWindow &window,
                                     std::string output_file_name)
        : Controller(), view(width, height, window, output_file_name),
          model(width, height, prob_alive, sexual, min_mutation, max_mutation,
                ideal_color, view),
          red_change(0), green_change(0), blue_change(0) {
} // ControllerAverage()

void ControllerAverage::display_view() {
    view.display();
} // display_view()

// update the game board if we aren't paused
void ControllerAverage::update_model() {
    if (!is_paused) {
        model.update();
    } // if
} // update_model()

// respond to key presses
void ControllerAverage::on_key_press(const sf::Event &event) {
    // toggle pause
    if (event.key.code == sf::Keyboard::P) {
        is_paused = !is_paused;
    } // if

    // toggle whether we are recording data
    if (event.key.code == sf::Keyboard::N) {
        view.toggle_recording();
        model.toggle_recording();
    } // if

    // if we are paused, step through generations one by one
    if (is_paused && event.key.code == sf::Keyboard::U) {
        model.update();
    } // if

    // increase the red channel of the ideal color
    if (event.key.code == sf::Keyboard::A) {
        red_change += COLOR_DELTA;
        if (red_change >= 0x000001) {
            model.increase_ideal_color(static_cast<int>(red_change), 0, 0);
            red_change = 0;
            model.update_stats();
        }
    } // if

    // decrease the red channel of the ideal color
    if (event.key.code == sf::Keyboard::Z) {
        red_change -= COLOR_DELTA;
        if (red_change <= -0x000001) {
            model.increase_ideal_color(static_cast<int>(red_change), 0, 0);
            red_change = 0;
            model.update_stats();
        }
    } // if

    // increase the green channel of the ideal color
    if (event.key.code == sf::Keyboard::S) {
        green_change += COLOR_DELTA;
        if (green_change >= 0x000001) {
            model.increase_ideal_color(0, static_cast<int>(green_change), 0);
            green_change = 0;
            model.update_stats();
        }
    } // if

    // decrease the green channgel of the ideal color
    if (event.key.code == sf::Keyboard::X) {
        green_change -= COLOR_DELTA;
        if (green_change <= -0x000001) {
            model.increase_ideal_color(0, static_cast<int>(green_change), 0);
            green_change = 0;
            model.update_stats();
        }
    } // if

    // increase the blue channel of the ideal color
    if (event.key.code == sf::Keyboard::D) {
        blue_change += COLOR_DELTA;
        if (blue_change >= 0x000001) {
            model.increase_ideal_color(0, 0, static_cast<int>(blue_change));
            blue_change = 0;
            model.update_stats();
        }
    } // if

    // decrease the blue channel of the ideal color
    if (event.key.code == sf::Keyboard::C) {
        blue_change -= COLOR_DELTA;
        if (blue_change <= -0x000001) {
            model.increase_ideal_color(0, 0, static_cast<int>(blue_change));
            blue_change = 0;
            model.update_stats();
        }
    } // if

    // set the ideal color to red
    if (event.key.code == sf::Keyboard::F) {
        model.set_ideal_color(0xFF0000);
        model.update_stats();
    } // if

    // set the ideal color to white
    if (event.key.code == sf::Keyboard::G) {
        model.set_ideal_color(0xFFFFFF);
        model.update_stats();
    } // if

    // set the ideal color to blue
    if (event.key.code == sf::Keyboard::H) {
        model.set_ideal_color(0x0000FF);
        model.update_stats();
    } // if

    // reset the board
    if (event.key.code == sf::Keyboard::R) {
        model.reset();
    } // if

} // on_key_press()

// set the ideal color used for determining fitness
void ControllerAverage::set_ideal_color(int ideal_color) {
    model.set_ideal_color(static_cast<unsigned int>(ideal_color));
}

} // namespace GameOfLife
