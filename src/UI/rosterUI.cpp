#include "UI/rosterUI.h"

RosterUI::RosterUI(RosterUIConfig config)
    : _config(config) {
}

void RosterUI::addUnit(uint32_t id) {
    _units.emplace_back(id);
}

void RosterUI::addConstruction(uint32_t id) {
    _constructions.emplace_back(id);
}

void RosterUI::removeUnit(uint32_t id) {
    _units.remove(id);
}

void RosterUI::removeConstruction(uint32_t id) {
    _constructions.remove(id);
}
