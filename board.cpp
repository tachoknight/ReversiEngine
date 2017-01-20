#include "board.hpp"

Board::Board() {
    std::cout << "Creating board..\n";
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
    cout << "Setting piece at " << getBoardPositionDescription(row, column) << " to " << getColor(color) << "\n";
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

vector<tuple<ROW, COLUMN>> Board::getValidMoves(COLOR color) {
    vector<tuple<ROW, COLUMN>> validMoves;

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

void Board::getAvailableSpacesAroundSpace(ROW row, COLUMN column, vector<tuple<ROW, COLUMN>>& availableSpaces) {
    cout << "Finding playable spots around " << getBoardPositionDescription(row, column) << "\n";
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
            cout << space->getDescription() << "\n";

            if (std::get<0>(space->getDisc()) == false) {
                availableSpaces.push_back(std::make_tuple(static_cast<ROW>(rowPos + 1), static_cast<COLUMN>(colPos + 1)));
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
