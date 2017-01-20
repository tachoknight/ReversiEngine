#include "player.hpp"

void Player::playTurn() {
    // Find out what our best play is
    auto bestPlay = evaluate();

    // And add it to our list of plays..
    this->turnSequences.push_back(bestPlay);

    // And play it
    play(bestPlay);
}

deque<shared_ptr<Space>> Player::evaluate() {
    cout << getColor(this->color) << " is evaluating a turn\n";

    auto validMoves = board.getValidMoves(this->color);

    cout << getColor(this->color) << " has these empty spots to evaluate:\n";
    std::for_each(validMoves.begin(), validMoves.end(), [this](tuple<ROW, COLUMN> pos) {
        cout << "\t" << getBoardPositionDescription(std::get<0>(pos), std::get<1>(pos)) << "\n";
        shared_ptr<Evaluator> evaluator = make_shared<Evaluator>(this->board, this->color);
        this->evaluators.push_back(evaluator);
        this->evaluatorThreads.push_back(thread(&Evaluator::evaluate, evaluator, std::get<0>(pos), std::get<1>(pos)));
    });

    // And wait for the job to finish
    std::for_each(this->evaluatorThreads.begin(), this->evaluatorThreads.end(), [](thread & t) {
        t.join();
    });

    // Now let's go through our possible moves and see what we can do
    deque<shared_ptr<Space>> bestSequence = this->analyze();

    this->cleanupEvaluators();

    return bestSequence;
}

deque<shared_ptr<Space>> Player::analyze() {
    // This is the sequence we'll play
    deque<shared_ptr<Space>> bestSequence;

    // Now let's go through each evaluator and see what plays we can make
    for_each(this->evaluators.begin(), this->evaluators.end(), [this, &bestSequence](shared_ptr<Evaluator> evaluator) {
        cout << "Looking at what we can do with " << evaluator->getEvaluatedSpace()->getDescription() << ".\n";
        auto sequenceCount = evaluator->getValidSequencesCount();

        if (sequenceCount == 0) {
            cout << "\tThere were no valid sequences for this one.\n";
        } else {
            cout << "\tWe have " << sequenceCount << " sequence" << (sequenceCount > 1 ? "s" : "") << " to analyze\n";

            // Okay, let's get the sequences and find out which one has the most spaces and
            // replace the existing bestSequence with that one if it turns out to be bigger
            auto sequences = evaluator->getValidSequences();
            for_each(sequences.begin(), sequences.end(), [&bestSequence, this](deque<shared_ptr<Space>> testSequence) {
                if (testSequence.size() > bestSequence.size()) {
                    bestSequence = testSequence;
                } else if (bestSequence.size() == testSequence.size()) {
                    // They're the same, so let's throw some randomness into
                    // it to mix it up a bit
                    int rando = getRandomTieBreaker(1, 2);

                    if (rando == 1) {
                        bestSequence = testSequence;
                    }
                }
            });
        }
    });

    return bestSequence;
}

void Player::cleanupEvaluators() {
    this->evaluators.clear();
    this->evaluatorThreads.clear();
}

void Player::play(deque<shared_ptr<Space>>& sequence) {
    cout << getColor(this->color) << " is playing a turn\n";

    // And set the color of our captured discs
    for_each(sequence.begin(), sequence.end(), [this](shared_ptr<Space> space) {
        this->board.setDisc(space->getRow(), space->getColumn(), this->color);
    });
}
