#include "disc.hpp"

string getColor(COLOR color) {
    string theColor;

    switch (to_integral(color)) {
        case 1:
            theColor = "Light";
            break;

        case 2:
            theColor = "Dark";
            break;

        case 3:
            theColor = "None";
            break;
    }

    return theColor;
}

void Disc::flipColor() {
    if (this->color == COLOR::DARK) {
        this->color = COLOR::LIGHT;
    }
    else {
        this->color = COLOR::DARK;
    }
}