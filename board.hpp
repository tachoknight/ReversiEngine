#pragma once

#include <vector>
#include <tuple>
#include <array>
#include <string>
#include <iostream>
#include <memory>
#include <cassert>
#include <sstream>
#include <set>
using std::array;
using std::vector;
using std::tuple;
using std::make_tuple;
using std::cout;
using std::shared_ptr;
using std::make_shared;
using std::stringstream;
using std::set;

#include "to_integral.hpp"
#include "space.hpp"
#include "dimensions.hpp"

class Board {
        array<array<shared_ptr<Space>, BOARD_DIMENSION>, BOARD_DIMENSION> gameBoard;
		vector<vector<string>> moves;
    public:
        Board();
        ~Board();
        void setDisc(ROW row, COLUMN column, COLOR color);
        bool boardFull();
        set<tuple<ROW, COLUMN>> getValidMoves(COLOR color);
        shared_ptr<Space> getSpace(ROW row, COLUMN column);
		void printBoard();
		void printScores();
		bool isAnyPlayerWipedOut();
		vector<vector<string>>& getGameMoves() {
			return moves;
		}
    private:
		tuple<int, int> getCurrentScores();
        void createBoardSpaces();
        void getAvailableSpacesAroundSpace(ROW row, COLUMN column, set<tuple<ROW, COLUMN>>& availableSpaces);
        int getRealRowNum(ROW row) {
            return to_integral(row) - 1;
        }
        int getRealColNum(COLUMN column) {
            return to_integral(column) - 1;
        }
};