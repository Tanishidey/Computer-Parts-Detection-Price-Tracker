#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include "Unit.h"
#include "Constant.h"

class Component : public Unit {
    std::string name;
public:
    Component(int compId): Unit(compId, (compId>=0 && compId<NUMBER_COMP)?CompIdCost[compId]:0) {
        if (compId>=0 && compId<NUMBER_COMP) name = CompId_nameMap[compId];
        else name = "Unknown";
    }
    const std::string& getName() const { return name; }
};

#endif
