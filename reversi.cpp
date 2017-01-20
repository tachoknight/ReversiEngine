#include <iostream>
using namespace std;

#include "game.hpp"

int main(int argc, char* argv[]) {

    Game game;
    game.play();
    game.showWinner();

    return 0;
}
