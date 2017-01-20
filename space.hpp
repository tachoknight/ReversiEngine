#pragma once

#include <tuple>
#include <string>
#include <memory>
using std::tuple;
using std::make_tuple;
using std::string;
using std::shared_ptr;
using std::make_shared;

#include "disc.hpp"
#include "dimensions.hpp"
#include "to_integral.hpp"

class Space {
        shared_ptr<Disc> disc;
        ROW row;
        COLUMN column;
    public:
        Space() : row(ROW::R_INVALID), column(COLUMN::C_INVALID) {}
        Space(ROW theRow, COLUMN theColumn) : row(theRow), column(theColumn) {}
        ~Space() {
            removeDisc();
        }
        ROW getRow() {
            return this->row;
        }
        COLUMN getColumn() {
            return this->column;
        }
        void addDisc(COLOR color);
        COLOR removeDisc();
        const tuple<bool, shared_ptr<Disc>> getDisc() const;
        string getDescription();
        bool hasDisc() const {
            return disc ? true : false;
        }
};
