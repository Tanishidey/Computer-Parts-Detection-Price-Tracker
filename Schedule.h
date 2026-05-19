#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include "Constant.h"
#include "Order.h"
using namespace std;

class Schedule {
protected:
    string bundlePath;
    vector<Order> allOrders, satisfied, cancelled;
    int totalCyclesUsed = 0;
    map<int,int> pcCount;    
    map<int,int> compCount;  

public:
    explicit Schedule(const string& path) : bundlePath(path) {}

    bool loadOrders() {  // was parseBundle
        ifstream in(bundlePath);
        if (!in) { cerr << "Failed to open: " << bundlePath << "\n"; return false; }
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

    void runSchedule() {
        totalCyclesUsed = 0; pcCount.clear(); compCount.clear();
        satisfied.clear(); cancelled.clear();

        int cyclesLeft = AVAIL_PROD_CYCLES;
        for (const auto& o : allOrders) {
            int pid = o.getPCId();
            int perUnit = PCIdReqCyc[pid];
            long long need = 1LL * perUnit * o.getQuantity();

            if (need <= cyclesLeft) {
                satisfied.push_back(o);
                cyclesLeft -= (int)need;
                totalCyclesUsed += (int)need;
                pcCount[pid] += o.getQuantity();
                for (int i = 0; i < NUMBER_COMP_PER_MODEL; ++i) {
                    int compId = PCIdReqComp[pid][i];
                    compCount[compId] += o.getQuantity();
                }
            } else {
                cancelled.push_back(o);
            }
        }
    }

    static void writeOrderList(const string& filename, const vector<Order>& v, const string& title) {
        ofstream out(filename);
        out << title << "\n\n";
        out << fixed << setprecision(2);
        for (const auto& o : v) {
            out << "Order ID: " << o.getOrderId()
                << " | PC ID: " << o.getPCId()
                << " | Qty: " << o.getQuantity()
                << " | Profit Rate: " << o.getProfitRate() << "% "
                << " | PCCost: $" << o.getPCCost()
                << " | Total Profit: $" << o.getTotalProfit()
                << "\n";
        }
    }

    void printReports(const string& outPrefix = "Schedule") { // was writeReports
        writeOrderList(outPrefix + "_AllOrders.txt", allOrders,   "All Orders");
        writeOrderList(outPrefix + "_Satisfied.txt",  satisfied,   "Satisfied Orders");
        writeOrderList(outPrefix + "_Cancelled.txt",  cancelled,   "Cancelled Orders");

        { ofstream out(outPrefix + "_PCBreakdown.txt");
          out << "PC Production Breakdown (PCId -> Units)\n\n";
          for (auto &kv : pcCount) out << "PC #" << kv.first << " : " << kv.second << "\n"; }

        { ofstream out(outPrefix + "_ComponentBreakdown.txt");
          out << "Component Requirements (CompId -> Units)\n\n";
          for (auto &kv : compCount)
              out << "Comp #" << kv.first << " (" << CompId_nameMap[kv.first] << ") : " << kv.second << "\n"; }

        double totalProfitNoDisc = 0.0;
        for (const auto& o : satisfied) totalProfitNoDisc += o.getTotalProfit();

        double cancelPenalty = 0.0;
        for (const auto& o : cancelled) {
            double unitProfit = o.getPCCost() * o.getProfitRate() / 100.0;
            double unitPrice  = o.getPCCost() + unitProfit;
            cancelPenalty += unitPrice * o.getQuantity() * (ORDER_CANCEL_PENALTY / 100.0);
        }

        ofstream out(outPrefix + "_Totals.txt");
        out << fixed << setprecision(2);
        out << "Total cycles used: " << totalCyclesUsed << " / " << AVAIL_PROD_CYCLES << "\n";
        out << "Total profit (no CPU discount): $" << totalProfitNoDisc << "\n";
        out << "Cancellation penalty (deduction): -$" << cancelPenalty << "\n";
        out << "Net profit (Task 3 base): $" << (totalProfitNoDisc - cancelPenalty) << "\n";
    }
};

#endif
