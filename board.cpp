#include "board.hpp"

Board::Board() {
	createBoardSpaces();

	// Set up the board in Othello style
	setDisc(ROW::R_4, COLUMN::C_D, COLOR::LIGHT);
	setDisc(ROW::R_4, COLUMN::C_E, COLOR::DARK);
	setDisc(ROW::R_5, COLUMN::C_D, COLOR::DARK);
	setDisc(ROW::R_5, COLUMN::C_E, COLOR::LIGHT);	
}

Board::~Board() {

}

void Board::createBoardSpaces() {
	for (int x = 0; x < BOARD_DIMENSION; ++x) {
		for (int y = 0; y < BOARD_DIMENSION; ++y) {
			this->gameBoard[x][y] = make_shared<Space>(static_cast<ROW>(x + 1), static_cast<COLUMN>(y + 1));
		}
	}
}

void Board::setDisc(ROW row, COLUMN column, COLOR color) {
	this->gameBoard[getRealRowNum(row)][getRealColNum(column)]->addDisc(color);
}

bool Board::boardFull() {
	for (auto rIt : this->gameBoard) {
		for (auto cIt : rIt) {
			if (std::get<0>(cIt->getDisc()) == false)
				return false;
		}
	}

	return true;
}

set<tuple<ROW, COLUMN>> Board::getValidMoves(COLOR color) {
	set<tuple<ROW, COLUMN>> validMoves;

	for (auto rIt : this->gameBoard) {
		for (auto cIt : rIt) {
			if (std::get<0>(cIt->getDisc()) == true) {
				shared_ptr<Disc> disc = std::get<1>(cIt->getDisc());
				ROW row = cIt->getRow();
				COLUMN column = cIt->getColumn();

				if (disc->getColor() != color) {
					// the disc is the opposite color, so it's potentially playable, so let's
					// see what's available around it
					getAvailableSpacesAroundSpace(row, column, validMoves);
				}
			}
		}
	}

	return validMoves;
}

void Board::getAvailableSpacesAroundSpace(ROW row, COLUMN column, set<tuple<ROW, COLUMN>>& availableSpaces) {
	int realRow = getRealRowNum(row);
	int realCol = getRealColNum(column);

	int startRow = realRow - 1;

	if (startRow < 0) {
		startRow = 0;
	}

	int endRow = realRow + 1;

	if (endRow == BOARD_DIMENSION) {
		endRow = BOARD_DIMENSION - 1;
	}

	int startCol = realCol - 1;

	if (startCol < 0) {
		startCol = 0;
	}

	int endCol = realCol + 1;

	if (endCol == BOARD_DIMENSION) {
		endCol = BOARD_DIMENSION - 1;
	}

	for (int rowPos = startRow; rowPos <= endRow; rowPos++) {
		for (int colPos = startCol; colPos <= endCol; colPos++) {
			shared_ptr<Space> space = this->gameBoard[rowPos][colPos];

			if (std::get<0>(space->getDisc()) == false) {
				availableSpaces.insert(std::make_tuple(static_cast<ROW>(rowPos + 1), static_cast<COLUMN>(colPos + 1)));
			}
		}
	}
}

shared_ptr<Space> Board::getSpace(ROW row, COLUMN column) {
	int realRow = getRealRowNum(row);
	int realCol = getRealColNum(column);

	assert(realRow >= 0 && realRow < BOARD_DIMENSION);
	assert(realCol >= 0 && realCol < BOARD_DIMENSION);

	return this->gameBoard[realRow][realCol];
}

void Board::printBoard() {
	string horizontalLine = string(1, ' ') + string(17, '-');
	vector<string> board;

	board.push_back("  A B C D E F G H ");
	board.push_back(horizontalLine);

	for (int x = 0; x < BOARD_DIMENSION; ++x) {
		stringstream rowToPrint;
		rowToPrint << (x + 1) << "|";
		for (int y = 0; y < BOARD_DIMENSION; ++y) {
			shared_ptr<Space> space = this->gameBoard[x][y];

			if (std::get<0>(space->getDisc()) == false) {
				rowToPrint << ".";
			}
			else {
				auto disc = std::get<1>(space->getDisc());
				switch (disc->getColor()) {
				case COLOR::DARK:
					rowToPrint << "X";
					break;
				case COLOR::LIGHT:
					rowToPrint << "O";
					break;
				}
			}

			if (y + 1 < BOARD_DIMENSION) {
				rowToPrint << " ";
			}
		}

		rowToPrint << "|";
		board.push_back(rowToPrint.str());
	}

	board.push_back(horizontalLine);

	// Save off the board state so we can replay the whole thing later
	this->moves.push_back(board);
}

tuple<int, int> Board::getCurrentScores() {
	int lightCount = 0;
	int darkCount = 0;

	for (int x = 0; x < BOARD_DIMENSION; ++x) {
		for (int y = 0; y < BOARD_DIMENSION; ++y) {
			shared_ptr<Space> space = this->gameBoard[x][y];

			if (std::get<0>(space->getDisc()) == false) {
				continue;
			}
			else {
				auto disc = std::get<1>(space->getDisc());
				switch (disc->getColor()) {
				case COLOR::DARK:
					++darkCount;
					break;
				case COLOR::LIGHT:
					++lightCount;
					break;
				}
			}
		}
	}

	return std::make_tuple(lightCount, darkCount);
}

void Board::printScores() {
	auto scores = this->getCurrentScores();

	cout << "Light Count: " << std::get<0>(scores) << " - Dark Count: " << std::get<1>(scores) << std::endl;
}

bool Board::isAnyPlayerWipedOut() {
	auto scores = this->getCurrentScores();

	// Is anyone at 0?
	if (std::get<0>(scores) == 0) {
		cout << "!!! WHITE IS WIPED OUT! !!!" << std::endl;
		return true;
	}
	else if (std::get<1>(scores) == 0) {
		cout << "!!! BLACK IS WIPED OUT! !!!" << std::endl;
		return true;
	}

	return false;
}