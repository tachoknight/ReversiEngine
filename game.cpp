#include "game.hpp"

void Game::play() {
	int turnCount = 0;
    do {
        Player& currentPlayer = getCurrentPlayer();
        currentPlayer.playTurn();		
		board.printBoard();
		board.printScores();
		// Check what the current stats are of the board
		// and if one of the players has been wiped out,
		// the game ends
		if (board.isAnyPlayerWipedOut()) {
			break;
		}
        nextTurn();
    } while (board.boardFull() == false);
}

void Game::nextTurn() {
    if (this->currentTurnColor == COLOR::LIGHT) {
        this->currentTurnColor = COLOR::DARK;
    }
    else {
        this->currentTurnColor = COLOR::LIGHT;
    }
}

void Game::showWinner() {
    int darkDiscs = 0;
    int lightDiscs = 0;

    for (int x = 0; x < BOARD_DIMENSION; ++x) {
        for (int y = 0; y < BOARD_DIMENSION; ++y) {
            shared_ptr<Space> space = this->board.getSpace(static_cast<ROW>(x + 1), static_cast<COLUMN>(y + 1));
            assert(space->hasDisc());

            if (std::get<1>(space->getDisc())->getColor() == COLOR::DARK) {
                ++darkDiscs;
            }
            else if (std::get<1>(space->getDisc())->getColor() == COLOR::LIGHT) {
                ++lightDiscs;
            }
            else {
                // Should not have anything else on the board
                assert(false);
            }
        }
    }

    cout << "Light: " << lightDiscs << " - Dark: " << darkDiscs << "\n";
    string winner;

    if (lightDiscs > darkDiscs) {
        winner = "LIGHT";
    }
    else if (darkDiscs > lightDiscs) {
        winner = "DARK";
    }
    else {
        winner = "TIE!!!";
    }

    cout << "\t**** " << winner << " HAS WON!! ****\n";
}

Player& Game::getCurrentPlayer() {
    if (this->currentTurnColor == COLOR::LIGHT) {
        return this->light;
    }

    return this->dark;
}

void Game::showMoves() {
	auto moves = this->board.getGameMoves();

	int moveNum = 0;
	for_each(moves.begin(), moves.end(), [&moveNum](vector<string> turn) {
		cout << "***** TURN " << (++moveNum) << std::endl;
		for_each(turn.begin(), turn.end(), [](string row) {
			cout << "\t" << row << std::endl;
		});
	});
}