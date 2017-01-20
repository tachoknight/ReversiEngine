#include "dimensions.hpp"

DIRECTION operator++(DIRECTION& direction) {
    return direction = (DIRECTION)(std::underlying_type<DIRECTION>::type(direction) + 1);
}

DIRECTION operator*(DIRECTION direction) {
    return direction;
}

DIRECTION begin(DIRECTION direction) {
    return DIRECTION::First;
}

DIRECTION end(DIRECTION direction) {
    DIRECTION d = DIRECTION::Last;
    return ++d;
}

// This function is done with the row and position as refers to the board; there is no
// translation to what would be used to reference the index of the board array. So if
// we're at row 1, column 1, that's the upper-left corner and so if we want the NW values,
// that would be 0, 0, which would be off the board (because when we translate to the board
// array, that would be -1 which is not a valid array position).
tuple<bool, ROW, COLUMN> getOffsetPosition(ROW row, COLUMN column, DIRECTION direction) {
    int offsetRow = to_integral(row);
    int offsetCol = to_integral(column);
    bool isValidPosition = true;

    switch (direction) {
        case DIRECTION::NW:
            offsetRow -= 1;
            offsetCol -= 1;
            break;

        case DIRECTION::N:
            offsetRow -= 1;
            break;

        case DIRECTION::NE:
            offsetRow -= 1;
            offsetCol += 1;
            break;

        case DIRECTION::W:
            offsetCol -= 1;
            break;

        case DIRECTION::E:
            offsetCol += 1;
            break;

        case DIRECTION::SW:
            offsetRow += 1;
            offsetCol -= 1;
            break;

        case DIRECTION::S:
            offsetRow += 1;
            break;

        case DIRECTION::SE:
            offsetRow += 1;
            offsetCol += 1;
            break;
    }

    if (offsetRow <= 0 || offsetCol <= 0) {
        isValidPosition = false;
    }
    else if (offsetRow > BOARD_DIMENSION || offsetCol > BOARD_DIMENSION) {
        isValidPosition = false;
    }

    return make_tuple(isValidPosition, static_cast<ROW>(offsetRow), static_cast<COLUMN>(offsetCol));
}

string getDirectionDescription(DIRECTION direction) {
    string desc;

    switch (direction) {
        case DIRECTION::NW:
            desc = "NW";
            break;

        case DIRECTION::N:
            desc = "N";
            break;

        case DIRECTION::NE:
            desc = "NE";
            break;

        case DIRECTION::W:
            desc = "W";
            break;

        case DIRECTION::E:
            desc = "E";
            break;

        case DIRECTION::SW:
            desc = "SW";
            break;

        case DIRECTION::SE:
            desc = "SE";
            break;

        case DIRECTION::S:
            desc = "S";
            break;
    }

    return desc;
}

string getBoardPositionDescription(ROW row, COLUMN column) {
    string pos;

    switch (column) {
        case COLUMN::C_A:
            pos = "A";
            break;

        case COLUMN::C_B:
            pos = "B";
            break;

        case COLUMN::C_C:
            pos = "C";
            break;

        case COLUMN::C_D:
            pos = "D";
            break;

        case COLUMN::C_E:
            pos = "E";
            break;

        case COLUMN::C_F:
            pos = "F";
            break;

        case COLUMN::C_G:
            pos = "G";
            break;

        case COLUMN::C_H:
            pos = "H";
            break;

        case COLUMN::C_INVALID:
            pos = "INVALID";
            break;
    }

    pos.append(std::to_string(to_integral(row)));

    return pos;
}
