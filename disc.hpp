#pragma once

#include <string>
using std::string;

#include "to_integral.hpp"

enum class COLOR : int {
    LIGHT = 1,
    DARK = 2,
    NONE = 3,
};

string getColor(COLOR color);

class Disc {
        COLOR color;
    public:
        Disc(COLOR newColor) : color(newColor) {}
        const COLOR& getColor() const {
            return this->color;
        }
        void setColor(COLOR newColor) {
            this->color = newColor;
        }
        void flipColor();
    private:
        // Must initialize with a color
        Disc() {}
};