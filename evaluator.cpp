#include "evaluator.hpp"

void Evaluator::evaluate(ROW row, COLUMN column) {
    auto threadID = std::this_thread::get_id();
    cout << "Thread " << threadID << " is going to evaluate "
         << getBoardPositionDescription(row, column) << "\n";
    this->baseSpace = board.getSpace(row, column);

    // We have to get all the valid playable positions around us. Empty spaces
    // around us
    // are not valid for evaluation
    for (const auto& pos : DIRECTION()) {
        auto offset = getOffsetSpace(row, column, pos);

        if (std::get<0>(offset)) {
            if (std::get<1>(offset)->hasDisc()) {
                cout << "We will check out " << std::get<1>(offset)->getDescription()
                     << "\n";
                validSpaces.push_back(EvaluatableSpace(std::get<1>(offset), pos));
            }
        }
    }

    // Now go through the vector and figure out what we can play on
    this->findValidSequences();
}

// Find all the valid sequences we can play on for each of the valid spaces
void Evaluator::findValidSequences() {
    cout << "=== START Finding Valid Sequences ===\n";

    // for each of the valid spaces we can possibly play on (i.e. there is a disc,
    // not a space)...
    for_each(
        this->validSpaces.begin(), this->validSpaces.end(),
    [this](EvaluatableSpace space) {
        cout << "Okay, let's see if we can start at "
             << space.validSpace->getDescription() << " and go "
             << getDirectionDescription(space.direction) << " to find a play\n";

        // First thing we have to do is check whether the current space we're
        // evaluating is the
        // opposite color or not. If it is *not* the opposite color, then we
        // can't play on it
        auto startSpaceColor = this->hasAndIsSameColor(space.validSpace);

        if (std::get<0>(startSpaceColor) == true &&
                std::get<1>(startSpaceColor) == true) {
            // There is a disc here, but it's ours, so we can't play on it
            cout << space.validSpace->getDescription() << " is the same color as "
                 << getColor(this->evaluatorColor)
                 << " so we can't go this way.\n";
        }
        else {
            //
            // Ah, okay, so we have the opposite color, which means we can start
            // going this way and see if we can find our
            // color as an end. Note that we may not; we may hit an empty space or
            // the end of the board, in which case
            // this will not be a valid sequence
            //

            // This is the sequence of possible moves we may be able to play on
            deque<shared_ptr<Space>> spaceSequence;
            // And this is our flag to determine whether we should add the vector
            // we make to the main vector
            bool okayToAdd = false;

            // Our flag that determines if we're at the end
            bool foundEnd = false;
            // Our working row
            ROW workingRow = space.validSpace->getRow();
            // Our working column
            COLUMN workingColumn = space.validSpace->getColumn();

            // The position we're going to evaluate; set here initially
            // to the first position we want to check
            auto nextPos = std::make_tuple(true, workingRow, workingColumn);

            // Our test space
            auto testSpace =
                board.getSpace(std::get<1>(nextPos), std::get<2>(nextPos));

            // Now keep going until we find some kind of end
            while (foundEnd == false) {
                cout << "Looking at "
                     << getBoardPositionDescription(std::get<1>(nextPos),
                                                    std::get<2>(nextPos))
                     << "\n";

                //
                // Test 1:
                // Are we at an invalid position (i.e. off the board?)
                //
                if (std::get<0>(nextPos) == false) {
                    cout << "Walked off the board, so we're done.\n";
                    // yes, so we're done
                    foundEnd = true;
                    break;
                }

                cout << "Looking at space " << testSpace->getDescription() << "\n";

                //
                // Test 2:
                // Is there a disc here?
                //
                if (testSpace->hasDisc() == false) {
                    // Didn't find a disc, so we're done
                    cout << "No disc at " << testSpace->getDescription()
                         << " so we're done.\n";
                    foundEnd = true;
                    break;
                }

                // Our test disc which we know is there
                auto testDisc = std::get<1>(testSpace->getDisc());

                //
                // Test 3:
                // There's a disc. It it our color, or their color?
                //
                if (testDisc->getColor() == this->evaluatorColor) {
                    // It's our color, so we're done
                    cout << "Test disc is same color as us ("
                         << getColor(this->evaluatorColor) << ") so we're done.\n";
                    // So we add this space to our vector as it will be the end
                    spaceSequence.push_back(testSpace);

                    foundEnd = true;

                    // And signal that we are okay to add this vector to the main
                    // evaluation
                    // vector
                    okayToAdd = true;
                    break;
                }
                else {
                    cout << "Test disc is opposite color as us ("
                         << getColor(this->evaluatorColor)
                         << ") so we're adding it.\n";
                    // It is their color, so we want to add it to the vector and keep
                    // going
                    spaceSequence.push_back(testSpace);
                }

                //
                // And now set the working row and column so we can continue on
                //

                // Now move to the next position on the board
                nextPos =
                    getOffsetPosition(workingRow, workingColumn, space.direction);

                // If we hit the end, stop
                if (std::get<0>(nextPos) == false) {
                    foundEnd = true;
                }
                else {
                    // We're not at the end, so keep going
                    workingRow = std::get<1>(nextPos);
                    workingColumn = std::get<2>(nextPos);

                    // And check out the next space
                    testSpace = board.getSpace(workingRow, workingColumn);
                }
            }

            if (okayToAdd) {
                cout << "Adding this sequence to the list of sequences...\n";

                // And put the space we were evaluating at the top, so that it will
                // be set as well
                spaceSequence.push_front(this->getEvaluatedSpace());

                // And finally add it to our list of sequences
                this->validSequences.push_back(spaceSequence);
            }
        }
    });

    cout << "We have " << this->validSequences.size()
         << " sequences to evaluate.\n";

    cout << "=== END Finding Valid Sequences ===\n";
}

// Returns a bool that the space is valid, and the space itself (if bool is
// false, space
// is 'invalid')
tuple<bool, shared_ptr<Space>> Evaluator::getOffsetSpace(ROW row, COLUMN column,
DIRECTION direction) {
    auto offsetSpace = getOffsetPosition(row, column, direction);

    if (std::get<0>(offsetSpace)) {
        return std::make_tuple(
                   true,
                   board.getSpace(std::get<1>(offsetSpace), std::get<2>(offsetSpace)));
    }

    // Nope, not a valid offset
    return std::make_tuple(false, make_shared<Space>());
}

tuple<bool, bool> Evaluator::hasAndIsSameColor(const shared_ptr<Space>& space) {
    // Do we have a disc?
    if (space->hasDisc() == false) {
        // We don't have a disc, so there's no color
        return make_tuple(false, false);
    }

    // Now we know we have a disc, so what color?
    if (std::get<1>(space->getDisc())->getColor() == this->evaluatorColor) {
        // Same color as the player color that's is doing the evaluating
        return make_tuple(true, true);
    }

    // Disc does not have the same color as the player
    return make_tuple(true, false);
}
