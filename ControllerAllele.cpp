/*
 * ControllerAllele.cpp
 * A controller class to handle the Allele game mode.
 */
#include "ControllerAllele.hpp"

namespace GameOfLife {

// construct an AlleleController
ControllerAllele::ControllerAllele(unsigned int width, unsigned int height,
                                   double prob_alive, double dom_frequency,
                                   sf::RenderWindow &window,
                                   std::string output_file_name)
        : Controller(), width(width), height(height), window(window),
          view(width, height, window, output_file_name),
          model(width, height, prob_alive, dom_frequency, view) {
} // ControllerAllele()

// display the view of the current board
void ControllerAllele::display_view() {
    view.display();
} // display_view()

// update the underlying board
void ControllerAllele::update_model() {
    if (!is_paused) {
        model.update();
    } // if
} // update_model()

// handle key press events
void ControllerAllele::on_key_press(const sf::Event &event) {
    // pause and unpase the game
    if (event.key.code == sf::Keyboard::P) {
        is_paused = !is_paused;
    } // if

    // toggle whether data is currently being recorded
    if (event.key.code == sf::Keyboard::N) {
        view.toggle_recording();
        model.toggle_recording();
    } // if

    // if paused, step through a single generation
    if (is_paused && event.key.code == sf::Keyboard::U) {
        model.update();
    } // if

    // reset the game board if R is pressed
    if (event.key.code == sf::Keyboard::R) {
        model.reset();
    } // if

} // on_key_press()

// update the titlebar to show the genotype of the cell currently under the
// mouse
void ControllerAllele::update_geno() {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    mouse_pos.x = static_cast<int>(mouse_pos.x * static_cast<double>(width) /
                                   window.getSize().x);
    mouse_pos.y = static_cast<int>(mouse_pos.y * static_cast<double>(height) /
                                   window.getSize().y);
    std::string geno = model.get_geno(static_cast<unsigned int>(mouse_pos.x),
                                      static_cast<unsigned int>(mouse_pos.y));
    geno += " (";
    geno += std::to_string(mouse_pos.x);
    geno += ", ";
    geno += std::to_string(mouse_pos.y);
    geno += ")";
    view.update_title(geno);
} // update_geno()

} // namespace GameOfLife
