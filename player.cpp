#include "player.hpp"

void Player::playTurn() {
    // Find out what our best play is
    auto bestPlay = evaluate();

    // And add it to our list of plays..
    this->turnSequences.push_back(bestPlay);

    // And play it
    play(bestPlay);
}

vector<deque<shared_ptr<Space>>> Player::evaluate() {
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
    vector<deque<shared_ptr<Space>>> bestSequence = this->analyze();

    this->cleanupEvaluators();

    return bestSequence;
}

vector<deque<shared_ptr<Space>>> Player::analyze() {
    // This is the sequence we'll play
    vector<deque<shared_ptr<Space>>> bestSeries;
	
	int bestScore = 0;
    // Now let's go through each evaluator and see what plays we can make
    for_each(this->evaluators.begin(), this->evaluators.end(), [this, &bestScore, &bestSeries](shared_ptr<Evaluator> evaluator) {
        auto sequenceCount = evaluator->getValidSequencesCount();

        if (sequenceCount != 0) {
			// Okay, let's get each of the sequences and find out which one has the most spaces
			// and that will be added to the score
			auto testSeries = evaluator->getValidSequences();
			int score = (int)testSeries.size();
			int seqScore = 0;
			for_each(testSeries.begin(), testSeries.end(), [&testSeries, &seqScore, this](deque<shared_ptr<Space>> testSequence) {
                seqScore += (int)testSequence.size();
                // Let's test for a possible corner position, which is one of the key spots
                // in the game
                for_each(testSequence.begin(), testSequence.end(), [&seqScore](shared_ptr<Space> space) {
					if ((space->getColumn() == COLUMN::C_A && space->getRow() == ROW::R_1)
						|| (space->getColumn() == COLUMN::C_H && space->getRow() == ROW::R_1)
						|| (space->getColumn() == COLUMN::C_A && space->getRow() == ROW::R_8)
						|| (space->getColumn() == COLUMN::C_H && space->getRow() == ROW::R_8))
					{
						// Ah, we can move to the corner, in which case we *really* want to
						// do that!
						seqScore *= 100;
					}
				});
			});
            // So now we calculate the total; we give ten points for each of the
            // rows in the vector (i.e. we can play multiple moves), and then we
            // add the number of discs that can affected by this particular
            // sequence
			int testScore = (score * 10) + seqScore;
			if (testScore > bestScore) {
				bestSeries = testSeries;
				bestScore = testScore;
			}
			else if (testScore == bestScore) {
				int rando = getRandomTieBreaker(1, 2);

				if (rando == 1) {
					bestSeries = testSeries;
					bestScore = testScore;
				}
			}
		}
    });

    return bestSeries;
}

void Player::cleanupEvaluators() {
    this->evaluators.clear();
    this->evaluatorThreads.clear();
}

void Player::play(vector<deque<shared_ptr<Space>>>& sequence) {
    // And set the color of our captured discs
    for_each(sequence.begin(), sequence.end(), [this](deque<shared_ptr<Space>> moves) {		
		for_each(moves.begin(), moves.end(), [this](shared_ptr<Space> move) {
			this->board.setDisc(move->getRow(), move->getColumn(), this->color);
		});
    });
}
