/*
 * Controller.hpp
 * A general class representing a controller for any game mode
 */

#ifndef Controller_hpp
#define Controller_hpp

#include "Model.hpp"
#include "View.hpp"
#include <SFML/System.hpp>
#include <stdio.h>
#include <string>
namespace GameOfLife {

class Controller {
public:
    // constructor
    Controller() : is_paused(true) {}

    // display the view
    virtual void display_view() = 0;

    // update the underlying model
    virtual void update_model() = 0;

    // act if key is pressed
    virtual void on_key_press(const sf::Event &event) = 0;

protected:
    bool is_paused;

}; // Controller

} // namespace GameOfLife
#endif /* Controller_hpp */
