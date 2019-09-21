/*
 *  main.cpp
 *  The main file for simulation. Reads in the settings and sets up the
 *  proper game mode. Default mode is Conway's Game of Life. Average
 *  mode is a mode where cells have a genetic code represented by color then
 * reproduce with parents passing on the average of their color to their
 * children. Allele mode has cells with an allele pair, and parents each pass
 * one allele to their child.
 */

#include "ControllerAllele.hpp"
#include "ControllerAverage.hpp"
#include "ControllerDefault.hpp"
#include "ResourcePath.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

static constexpr float DELAY_DELTA = 0.005f;
static float DELAY = 0;

// trim whitespace from an input string
static inline void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    [](int ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](int ch) { return !std::isspace(ch); })
                    .base(),
            s.end());
}

// struct to hold all the settings for the basic Game of Life mode
struct DefaultSettings {
    std::string output_file;
    unsigned int window_width;
    unsigned int window_height;
    unsigned int grid_width;
    unsigned int grid_height;
    double prob_alive;
};

// read in all the settings for the default game mode from is
DefaultSettings read_default(std::istream &is, const std::string &output) {
    DefaultSettings out;
    out.output_file = output;
    int idx = 0;
    std::string line;
    while (getline(is, line)) {
        // skip comments and empty lines
        if (line.empty() || line.compare(0, 2, "//") == 0) continue;

        // delete prompt
        line.erase(0, line.find_first_of(":") + 1);

        // strip trailing and leading whitespace
        trim(line);
        switch (idx) {
            case 0:
                out.window_width = static_cast<unsigned int>(std::stoi(line));
                break;
            case 1:
                out.window_height = static_cast<unsigned int>(std::stoi(line));
                break;
            case 2:
                out.grid_width = static_cast<unsigned int>(std::stoi(line));
                break;
            case 3:
                out.grid_height = static_cast<unsigned int>(std::stoi(line));
                break;
            case 4:
                out.prob_alive = std::stod(line);
                break;
        }
        ++idx;
    }
    return out;
} // read_default()

// the main loop for the default game mode
void main_default(std::istream &is) {
    // default doesn't currently record anything, so output file is just empty
    DefaultSettings set = read_default(is, std::string());
    // create
    sf::RenderWindow window(sf::VideoMode(set.window_width, set.window_height),
                            "0", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    GameOfLife::ControllerDefault controller{set.grid_width, set.grid_height,
                                             set.prob_alive, window,
                                             set.output_file};

    sf::Clock clock;
    clock.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                // exit the game
                case sf::Event::Closed:
                    window.close();
                    break;
                // spped up or slow down the evolution with left/right keys
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Right &&
                        DELAY >= DELAY_DELTA) {
                        DELAY -= DELAY_DELTA;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        DELAY += DELAY_DELTA;
                    }
                    controller.on_key_press(event);
                    break;

                default:
                    break;
            }
        }

        window.clear();

        if (clock.getElapsedTime().asSeconds() >= DELAY) {
            controller.update_model();
            clock.restart();
        }

        controller.display_view();

        window.display();
    }
} // main_default()

// struct to hold settings for average game mode
struct AverageSettings {
    std::string output_file;
    unsigned int window_width;
    unsigned int window_height;
    unsigned int grid_width;
    unsigned int grid_height;
    double prob_alive;
    unsigned int ideal_color;
    bool sexual;         // asexual or sexual reproduction
    double min_mutation; // mutation rate
    double max_mutation;
};

// read in the settings from is for average game mode
AverageSettings read_average(std::istream &is, const std::string &output) {
    AverageSettings out;
    out.output_file = output;
    int idx = 0;
    std::string line;
    while (getline(is, line)) {
        // skip comments and empty lines
        if (line.empty() || line.compare(0, 2, "//") == 0) continue;

        // delete prompt
        line.erase(0, line.find_first_of(":") + 1);

        // strip trailing and leading whitespace
        trim(line);

        switch (idx) {
            case 0:
                out.window_width = static_cast<unsigned int>(std::stoi(line));
                break;
            case 1:
                out.window_height = static_cast<unsigned int>(std::stoi(line));
                break;
            case 2:
                out.grid_width = static_cast<unsigned int>(std::stoi(line));
                break;
            case 3:
                out.grid_height = static_cast<unsigned int>(std::stoi(line));
                break;
            case 4:
                out.prob_alive = std::stod(line);
                break;
            case 5:
                std::transform(line.begin(), line.end(), line.begin(),
                               ::tolower);
                out.ideal_color =
                        static_cast<unsigned int>(std::stoi(line, nullptr, 16));
                break;
            case 6:
                out.sexual = std::tolower(line[0]) == 't';
                break;
            case 7:
                out.min_mutation = std::stod(line);
                break;
            case 8:
                out.max_mutation = std::stod(line);
                break;
        }
        ++idx;
    }
    return out;
} // read_average()

// main method for average mode
void main_average(std::istream &is, const std::string &output) {
    AverageSettings set = read_average(is, output);
    sf::RenderWindow window(sf::VideoMode(set.window_width, set.window_height),
                            "0", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    GameOfLife::ControllerAverage controller{
            set.grid_width,  set.grid_height,  set.prob_alive,
            set.sexual,      set.min_mutation, set.max_mutation,
            set.ideal_color, window,           set.output_file};

    sf::Clock clock;
    clock.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    // speed up or slow down the evolution with left/right keys
                    if (event.key.code == sf::Keyboard::Right &&
                        DELAY >= DELAY_DELTA) {
                        DELAY -= DELAY_DELTA;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        DELAY += DELAY_DELTA;
                    }
                    controller.on_key_press(event);
                    break;

                default:
                    break;
            }
        }

        window.clear();

        if (clock.getElapsedTime().asSeconds() >= DELAY) {
            controller.update_model();
            clock.restart();
        }

        controller.display_view();

        window.display();
    }
} // main_average()

// struct to hold settings for average game mode
struct AlleleSettings {
    std::string output_file;
    unsigned int window_width;
    unsigned int window_height;
    unsigned int grid_width;
    unsigned int grid_height;
    double prob_alive;
    double dom_frequency;
};

// read in the settings from is for average game mode
AlleleSettings read_allele(std::istream &is, const std::string &output) {
    AlleleSettings out;
    out.output_file = output;
    int idx = 0;
    std::string line;
    while (getline(is, line)) {
        // skip comments and empty lines
        if (line.empty() || line.compare(0, 2, "//") == 0) continue;

        // delete prompt
        line.erase(0, line.find_first_of(":") + 1);

        // strip trailing and leading whitespace
        trim(line);

        switch (idx) {
            case 0:
                out.window_width = static_cast<unsigned int>(std::stoi(line));
                break;
            case 1:
                out.window_height = static_cast<unsigned int>(std::stoi(line));
                break;
            case 2:
                out.grid_width = static_cast<unsigned int>(std::stoi(line));
                break;
            case 3:
                out.grid_height = static_cast<unsigned int>(std::stoi(line));
                break;
            case 4:
                out.prob_alive = std::stod(line);
                break;
            case 5:
                out.dom_frequency = std::stod(line);
                break;
        }
        ++idx;
    }
    return out;
} // read_average()

// main method for average mode
void main_allele(std::istream &is, const std::string &output) {
    AlleleSettings set = read_allele(is, output);

    sf::RenderWindow window(sf::VideoMode(set.window_width, set.window_height),
                            "0", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    GameOfLife::ControllerAllele controller{set.grid_width, set.grid_height,
                                            set.prob_alive, set.dom_frequency,
                                            window,         set.output_file};

    sf::Clock clock;
    clock.restart();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            controller.update_geno();
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Right &&
                        DELAY >= DELAY_DELTA) {
                        DELAY -= DELAY_DELTA;
                    } else if (event.key.code == sf::Keyboard::Left) {
                        DELAY += DELAY_DELTA;
                    }
                    controller.on_key_press(event);
                    break;

                default:
                    break;
            }
        }

        window.clear();

        if (clock.getElapsedTime().asSeconds() >= DELAY) {
            controller.update_model();
            clock.restart();
        }

        controller.display_view();

        window.display();
    }
} // main_allele()

// Read in input / output files from the command line, then dispatch
// to the appropriate game mode
int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: ./pixels SettingsFile [OutputFile]" << std::endl;
        exit(1);
    }

    std::ifstream in(argv[1]);
    std::string mode;
    in >> mode;

    if (mode == "Default") {
        main_default(in);
    } else if (mode == "Average") {
        if (argc != 3) {
            std::cerr << "Error: Must specify output file for Average mode."
                      << std::endl;
            exit(1);
        }
        main_average(in, std::string(argv[2]));
    } else if (mode == "Allele") {
        if (argc != 3) {
            std::cerr << "Error: Must specify output file for Allele mode."
                      << std::endl;
            exit(1);
        }
        main_allele(in, std::string(argv[2]));
    } else {
        std::cerr << "Error: Invalid game mode \"" << mode << "\"" << std::endl;
    }

    return EXIT_SUCCESS;
}
