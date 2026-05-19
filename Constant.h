#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <string>
using std::string;

const int NUMBER_COMP            = 10;
const int NUMBER_PC_MODELS       = 16;
const int NUMBER_COMP_PER_MODEL  = 4;

const int RUNNING_COST_PER_CYCLE = 100;
const int AVAIL_PROD_CYCLES      = 10000;

const int ORDER_BUNDLE_QUANT_MIN = 120;
const int ORDER_BUNDLE_QUANT_MAX = 200;

const int ORDER_QUANT_MIN        = 1;
const int ORDER_QUANT_MAX        = 30;

const int UNIT_PROFIT_RATE_MIN   = 45;
const int UNIT_PROFIT_RATE_MAX   = 75;

const int ORDER_CANCEL_PENALTY   = 3;  
const int DISCOUNT_PER_500_UNITS = 18;  

// names & costs
const string CompId_nameMap[NUMBER_COMP] = {
    "Pintel CPU 3GHz","Pintel CPU 5GHz","IMD CPU 3GHz","IMD CPU 5GHz",
    "Pintel motherboard","IMD motherboard","Memory 16GB","Memory 32GB",
    "Hard disk 1TB","Hard disk 2TB"
};

const int CompIdCost[NUMBER_COMP] = {1000,1500,1000,1500,250,250,100,200,300,400};

// modelof required components
const int PCIdReqComp[NUMBER_PC_MODELS][NUMBER_COMP_PER_MODEL] = {
    {0,4,6,8},{0,4,6,9},{0,4,7,8},{0,4,7,9},
    {1,4,6,8},{1,4,6,9},{1,4,7,8},{1,4,7,9},
    {2,5,6,8},{2,5,6,9},{2,5,7,8},{2,5,7,9},
    {3,5,6,8},{3,5,6,9},{3,5,7,8},{3,5,7,9}
};

// cycles mod
const int PCIdReqCyc[NUMBER_PC_MODELS] = {
    4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7
};

#endif 
