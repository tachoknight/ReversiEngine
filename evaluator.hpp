#pragma once
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include <deque>
using std::cout;
using std::tuple;
using std::make_tuple;
using std::vector;
using std::deque;

#include "board.hpp"
#include "dimensions.hpp"


// This is the class responsible for determining if a move is
// valid, and if it is, will hand off the path to an analysis
// class
class Evaluator {
        struct EvaluatableSpace {
            shared_ptr<Space> validSpace;
            DIRECTION direction;

            EvaluatableSpace(const shared_ptr<Space>& validSpace, DIRECTION direction) {
                this->validSpace = validSpace;
                this->direction = direction;
            }
        };

        Board& board;
        COLOR evaluatorColor;
        vector<EvaluatableSpace> validSpaces;
        shared_ptr<Space> baseSpace;

        // Our analysis stuff
        vector<deque<shared_ptr<Space>>> validSequences;
    public:
        Evaluator(Board& gameBoard, COLOR forColor) : board(gameBoard), evaluatorColor(forColor) {}
        void evaluate(ROW row, COLUMN column);
        shared_ptr<Space> getEvaluatedSpace() {
            return this->baseSpace;
        }
        size_t getValidSequencesCount() {
            return validSequences.size();
        }

        vector<deque<shared_ptr<Space>>>& getValidSequences() {
            return this->validSequences;
        }
    private:
        tuple<bool, shared_ptr<Space>> getOffsetSpace(ROW row, COLUMN column, DIRECTION direction);
        void findValidSequences();
        tuple<bool, bool> hasAndIsSameColor(const shared_ptr<Space>& space);
};

