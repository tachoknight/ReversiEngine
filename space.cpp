#include "space.hpp"

void Space::addDisc(COLOR color) {
    this->disc = make_shared<Disc>(color);
}

const tuple<bool, shared_ptr<Disc>> Space::getDisc() const {
    return make_tuple(this->hasDisc(), this->disc);
}

COLOR Space::removeDisc() {
    if (this->hasDisc()) {
        COLOR discColor = this->disc->getColor();

        return discColor;
    }

    return COLOR::NONE;
}

string Space::getDescription() {
    string description = getBoardPositionDescription(this->row, this->column);

    if (this->hasDisc()) {
        description.append(" Disc color: ");
        description.append(getColor(this->disc->getColor()));
    }
    else {
        description.append(" No disc");
    }

    return description;
}
