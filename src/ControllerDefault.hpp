//
//  ControllerDefault.hpp
//  GameOfLife
//
//  Created by Scott Guest on 3/17/19.
//  Copyright © 2019 Scott Guest. All rights reserved.
//

#ifndef ControllerDefault_hpp
#define ControllerDefault_hpp

#include "Controller.hpp"
#include "ModelDefault.hpp"
#include "ViewDefault.hpp"
#include <stdio.h>

namespace GameOfLife {

class ControllerDefault : public Controller {
public:
    ControllerDefault(unsigned int width, unsigned int height,
                      double prob_alive, sf::RenderWindow &window,
                      std::string output_file_name);
    // display the current board state
    void display_view() override;
    // update the underlying board
    void update_model() override;
    // respond to keyt presses
    void on_key_press(const sf::Event &event) override;

private:
    ViewDefault view;
    ModelDefault model;
};

} // namespace GameOfLife
#endif /* ControllerDefault_hpp */
