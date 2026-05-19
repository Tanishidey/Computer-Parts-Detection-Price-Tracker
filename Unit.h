#ifndef UNIT_H_
#define UNIT_H_

#include <string>
using namespace std;

// base class…
class Unit {
protected:
    int id;
    int cost; // total pc cpst for product incl cycles*running

public:
    Unit(int i = -1, int c = 0) : id(i), cost(c) {}

    int getId() const { return id; }

    int getCost() const { return cost; }

    // for changing price
    void setCost(int c) { cost = c; }

    virtual ~Unit() {} 
};

#endif // UNIT_H_
