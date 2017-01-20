#pragma once

#include <vector>
#include <tuple>
#include <array>
#include <string>
#include <iostream>
#include <memory>
#include <cassert>
using std::array;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::cout;
using std::shared_ptr;
using std::make_shared;

#include "to_integral.hpp"
#include "space.hpp"
#include "dimensions.hpp"

class Board {
        array<array<shared_ptr<Space>, BOARD_DIMENSION>, BOARD_DIMENSION> gameBoard;
    public:
        Board();
        ~Board();
        void setDisc(ROW row, COLUMN column, COLOR color);
        bool boardFull();
        vector<tuple<ROW, COLUMN>> getValidMoves(COLOR color);
        shared_ptr<Space> getSpace(ROW row, COLUMN column);
    private:
        void createBoardSpaces();
        void getAvailableSpacesAroundSpace(ROW row, COLUMN column, vector<tuple<ROW, COLUMN>>& availableSpaces);
        int getRealRowNum(ROW row) {
            return to_integral(row) - 1;
        }
        int getRealColNum(COLUMN column) {
            return to_integral(column) - 1;
        }
};