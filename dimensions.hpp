#pragma once
#include <iostream>
#include <tuple>
#include <string>
using std::tuple;
using std::make_tuple;
using std::string;

#include "to_integral.hpp"

const int BOARD_DIMENSION = 8;

enum class COLUMN : int {
    C_A = 1,
    C_B = 2,
    C_C = 3,
    C_D = 4,
    C_E = 5,
    C_F = 6,
    C_G = 7,
    C_H = 8,
    C_INVALID = 999,
};

enum class ROW : int {
    R_1 = 1,
    R_2 = 2,
    R_3 = 3,
    R_4 = 4,
    R_5 = 5,
    R_6 = 6,
    R_7 = 7,
    R_8 = 8,
    R_INVALID = 999,
};

enum class DIRECTION : int {
    NW,
    N,
    NE,
    W,
    E,
    S,
    SW,
    SE,
    First = NW,
    Last = SE,
};

DIRECTION operator++(DIRECTION& direction);
DIRECTION operator*(DIRECTION direction);
DIRECTION begin(DIRECTION direction);
DIRECTION end(DIRECTION direction);

tuple<bool, ROW, COLUMN> getOffsetPosition(ROW row, COLUMN column, DIRECTION direction);
string getBoardPositionDescription(ROW row, COLUMN column);
string getDirectionDescription(DIRECTION direction);
