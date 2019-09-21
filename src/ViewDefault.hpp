/*
 * ViewDefault.hpp
 * A view class to handle the default game mode.
 */
#ifndef ViewDefault_hpp
#define ViewDefault_hpp

#include "View.hpp"
#include <stdio.h>
namespace GameOfLife {

class ViewDefault : public View {
public:
    ViewDefault(unsigned int width, unsigned int height,
                sf::RenderWindow &window, std::string output_file_name);
    void update(unsigned int position, int color, std::uint64_t generations_);
    void display() override;
};

} // namespace GameOfLife
#endif /* ViewDefault_hpp */
