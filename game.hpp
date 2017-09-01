#pragma once

#include <algorithm>
using std::for_each;

#include "player.hpp"
#include "board.hpp"

class Player;

class Game {
        Board board;
        Player dark;
        Player light;
        COLOR currentTurnColor;
    public:
        Game() : dark(board, COLOR::DARK), light(board, COLOR::LIGHT), currentTurnColor(COLOR::DARK) {}
        void play();
        void showWinner();
		void showMoves();
    private:
        void nextTurn();
        Player& getCurrentPlayer();
};