#ifndef SMARTSCHEDULE_H_
#define SMARTSCHEDULE_H_

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
using namespace std;

#include "Schedule.h"   // brings in Constant.h + Order.h via Schedule

// Greedy optimizer: pick orders with highest profit per cycle,
// then add CPU-bulk discount (>=500 units of any CPU save 18% of CPU price).
class SmartSchedule : public Schedule {
private:
    // we keep our own copies so we don’t depend on Schedule’s internals
    vector<Order> allOrders;
    vector<Order> satisfied;
    vector<Order> cancelled;

    map<int,int> pcCount;     // PCId -> units produced
    map<int,int> compCount;   // CompId -> total units
    map<int,int> cpuCounts;   // CompId (0..3) -> units

    int    totalCyclesUsed      = 0;
    double totalProfitNoDisc    = 0.0;
    double totalProfitWithDisc  = 0.0;

public:
    explicit SmartSchedule(const string& path) : Schedule(path) {}

    // Load bundle via inherited bundlePath
    bool loadOrders() {
        ifstream in(bundlePath);
        if (!in) { cerr << "Failed to open bundle: " << bundlePath << "\n"; return false; }
        char c;
        while (in >> c) {
            if (c != '[') { in.unget(); string junk; getline(in, junk); continue; }
            int oid, pid, qty, upr; char comma, endc;
            in >> oid >> comma >> pid >> comma >> qty >> comma >> upr >> endc;
            if (!in) break;
            allOrders.emplace_back(oid, pid, qty, upr);
        }
        return true;
    }

    // profit with cpu cycle count
    void optimizeSchedule() {
        satisfied.clear(); cancelled.clear();
        pcCount.clear(); compCount.clear(); cpuCounts.clear();
        totalCyclesUsed = 0; totalProfitNoDisc = 0.0; totalProfitWithDisc = 0.0;

        struct Node { Order o; double ratio; int needCycles; };
        vector<Node> cand;
        cand.reserve(allOrders.size());
        for (const auto& o : allOrders) {
            int pid   = o.getPCId();
            int need  = PCIdReqCyc[pid] * o.getQuantity();
            double r  = (need > 0) ? (o.getTotalProfit() / need) : 0.0;
            cand.push_back({o, r, need});
        }
        sort(cand.begin(), cand.end(),
             [](const Node& a, const Node& b){ return a.ratio > b.ratio; });

        int cyclesLeft = AVAIL_PROD_CYCLES;
        for (const auto& n : cand) {
            const Order& o = n.o;
            if (n.needCycles <= cyclesLeft) {
                satisfied.push_back(o);
                cyclesLeft     -= n.needCycles;
                totalCyclesUsed += n.needCycles;
                totalProfitNoDisc += o.getTotalProfit();

                pcCount[o.getPCId()] += o.getQuantity();

                // component taly
                for (int i = 0; i < NUMBER_COMP_PER_MODEL; ++i) {
                    int compId = PCIdReqComp[o.getPCId()][i];
                    compCount[compId] += o.getQuantity();
                    if (compId >= 0 && compId <= 3) { // CPUs are 0..3
                        cpuCounts[compId] += o.getQuantity();
                    }
                }
            } else {
                cancelled.push_back(o);
            }
        }

        // 18%disc with with cpu >=5000
        double discountProfit = 0.0;
        for (const auto& kv : cpuCounts) {
            int compId = kv.first;
            int units  = kv.second;
            if (units >= 500) {
                double listPrice = CompIdCost[compId];
                double savePer   = listPrice * (DISCOUNT_PER_500_UNITS / 100.0);
                discountProfit  += savePer * units;
            }
        }
        totalProfitWithDisc = totalProfitNoDisc + discountProfit;

        // report 
        ofstream out("SmartSchedule_Report.txt");
        out << fixed << setprecision(2);
        out << "Smart Schedule Optimization\n\n";
        out << "Total cycles used: " << totalCyclesUsed << " / " << AVAIL_PROD_CYCLES << "\n";
        out << "Orders satisfied: " << satisfied.size() << "\n";
        out << "Orders cancelled: " << cancelled.size() << "\n";
        out << "Profit before CPU discount: $" << totalProfitNoDisc << "\n";
        out << "Additional CPU discount profit: $" << (totalProfitWithDisc - totalProfitNoDisc) << "\n";
        out << "Total profit (with discount): $" << totalProfitWithDisc << "\n\n";

        out << "CPU counts (for discount check):\n";
        for (const auto& kv : cpuCounts) {
            out << "  " << CompId_nameMap[kv.first] << " : " << kv.second << "\n";
        }

        out << "\nComponent breakdown:\n";
        for (const auto& kv : compCount) {
            out << "  " << setw(2) << kv.first << " - " << CompId_nameMap[kv.first]
                << " : " << kv.second << "\n";
        }
    }
};

#endif 
