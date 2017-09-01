#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <deque>
#include <random>
using std::cout;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::thread;
using std::deque;
using std::random_device;
using std::mt19937;

#include "board.hpp"
#include "evaluator.hpp"

class Player {
        Board& board;
        COLOR color;
        vector<shared_ptr<Evaluator>> evaluators;
        vector<thread> evaluatorThreads;

        // For storing plays
        vector<vector<deque<shared_ptr<Space>>>> turnSequences;
    public:
        Player(Board& gameBoard, COLOR playerColor) : board(gameBoard), color(playerColor) {}
        void playTurn();

    private:
		vector<deque<shared_ptr<Space>>> evaluate();
        vector<deque<shared_ptr<Space>>> analyze();
        void cleanupEvaluators();
        void play(vector<deque<shared_ptr<Space>>>& sequence);

        int getRandomTieBreaker(int min, int max) {
            random_device rng;
            mt19937 urng(rng());

            auto rando = std::bind(std::uniform_int_distribution<int>(min, max), urng);

            return rando();
        }
};
