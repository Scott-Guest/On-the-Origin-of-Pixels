/*
 * Model.hpp
 * A general class representing a model for any game mode
 */

#ifndef Model_hpp
#define Model_hpp

#include "View.hpp"
#include <cstdint>
#include <random>
#include <stdio.h>
#include <string>
#include <vector>

namespace GameOfLife {

class Model {
public:
    Model(unsigned int width, unsigned int height, double prob_alive)
            : rng(), width(width), height(height), generations(0u),
              num_alive(0), prob_alive(prob_alive), is_recording(false) {
        // seed random number generator
        std::random_device rand_dev;
        rng.seed(rand_dev());
    } // Model()

    // toggle recording on/off
    void toggle_recording() {
        is_recording = !is_recording;
    }

    // update the model if needed
    virtual void update() = 0;

    // reset the whole game
    virtual void reset() = 0;

protected:
    std::minstd_rand rng;

    unsigned int width;
    unsigned int height;
    unsigned int generations;
    unsigned int num_alive;
    double prob_alive;

    bool is_recording;
    static constexpr int IS_DEAD = 0x000000;
    static constexpr unsigned int IS_ALIVE = 0xFFFFFF;
};

} // namespace GameOfLife
#endif /* Model_hpp */
