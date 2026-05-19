#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <sstream>
#include "Unit.h"
#include "Constant.h"

class Product : public Unit {
    int cycles;
public:
    Product(int pcId): Unit(pcId, 0), cycles(0) {
        if (pcId>=0 && pcId<NUMBER_PC_MODELS) {
            cycles = PCIdReqCyc[pcId];
            int compCost = 0;
            for (int i=0;i<NUMBER_COMP_PER_MODEL;i++){
                int cid = PCIdReqComp[pcId][i];
                compCost += CompIdCost[cid];
            }
            cost = compCost + cycles*RUNNING_COST_PER_CYCLE;
        } else {
            cycles=0; cost=0;
        }
    }
    int getCycles() const { return cycles; }
    std::string brief() const {
        std::ostringstream os;
        os << "Product PC#" << id << " cost=" << cost << " cycles=" << cycles;
        return os.str();
    }
};

#endif
