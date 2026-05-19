#ifndef BUNDLEGENERATOR_H_
#define BUNDLEGENERATOR_H_

#include <fstream>
#include <random>
#include <string>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <iomanip>
#include "Constant.h"
using namespace std;

class BundleGenerator {
private:
    mt19937 rng;

    // inclusive integer
    static int urand(mt19937& r, int lo, int hi) {
        uniform_int_distribution<int> dist(lo, hi);
        return dist(r);
    }

public:
    BundleGenerator() {
        rng.seed(static_cast<unsigned long>(
            chrono::high_resolution_clock::now().time_since_epoch().count()));
    }

    void generateBundle(const string& filename, int count = -1, int startId = 100) {
        if (count <= 0) {
            count = urand(rng, ORDER_BUNDLE_QUANT_MIN, ORDER_BUNDLE_QUANT_MAX);
        }

        ofstream out(filename);
        if (!out) {
            throw runtime_error("Cannot open output file: " + filename);
        }
        const int pcMin = 0;
        const int pcMax = NUMBER_PC_MODELS - 1;  
        const int qtyMin = ORDER_QUANT_MIN;      
        const int qtyMax = ORDER_QUANT_MAX;      
        const int uprMin = UNIT_PROFIT_RATE_MIN; 
        const int uprMax = UNIT_PROFIT_RATE_MAX; 

        int oid = startId;

        for (int i = 0; i < count; ++i) {
            int PCId = urand(rng, pcMin, pcMax);
            int qty  = urand(rng, qtyMin, qtyMax);
            int upr  = urand(rng, uprMin, uprMax);
            out << "[" << oid << ", " << PCId << ", " << qty << ", " << upr << "]\n";
            ++oid;
        }

        out.close();
    }
};

#endif
