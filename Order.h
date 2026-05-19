#ifndef ORDER_H_
#define ORDER_H_

#include <iostream>
#include <iomanip>
#include "Constant.h"
using namespace std;

class Order {
private:
    int orderId;
    int PCId;
    int quantity;
    int unitProfitRate;
    double PCCost;
    double unitPCProfit;
    double unitPCPrice;
    double totalProfit;
    double unitPCProfitWithDiscount;
    double totalProfitWithDiscount;

public:
    Order() : orderId(0), PCId(0), quantity(0), unitProfitRate(0),
              PCCost(0), unitPCProfit(0), unitPCPrice(0),
              totalProfit(0), unitPCProfitWithDiscount(0), totalProfitWithDiscount(0) {}

    Order(int id, int pid, int q, int upr) {
        orderId = id; PCId = pid; quantity = q; unitProfitRate = upr;
        calculateCost();
    }

    void calculateCost() {
        PCCost = 0;
        for (int i = 0; i < NUMBER_COMP_PER_MODEL; ++i) {
            int compId = PCIdReqComp[PCId][i];
            PCCost += CompIdCost[compId];
        }
        PCCost += PCIdReqCyc[PCId] * RUNNING_COST_PER_CYCLE;

        unitPCProfit = PCCost * unitProfitRate / 100.0;
        unitPCPrice  = PCCost + unitPCProfit;
        totalProfit  = unitPCProfit * quantity;

        unitPCProfitWithDiscount = unitPCProfit;
        totalProfitWithDiscount  = totalProfit;
    }

    void applyDiscount(double discountRate) {
        double newCost = PCCost * (1.0 - discountRate / 100.0);
        unitPCProfitWithDiscount = newCost * unitProfitRate / 100.0;
        totalProfitWithDiscount  = unitPCProfitWithDiscount * quantity;
    }

    int    getOrderId()     const { return orderId; }
    int    getPCId()        const { return PCId; }
    int    getQuantity()    const { return quantity; }
    int    getProfitRate()  const { return unitProfitRate; }
    double getPCCost()      const { return PCCost; }
    double getTotalProfit() const { return totalProfit; }

    void display() const {
        cout << fixed << setprecision(2);
        cout << "Order ID: " << orderId
             << " | PC ID: " << PCId
             << " | Qty: " << quantity
             << " | Profit Rate: " << unitProfitRate << "%"
             << " | PC Cost: $" << PCCost
             << " | Unit Profit: $" << unitPCProfit
             << " | Total Profit: $" << totalProfit
             << "\n";
    }

    // decenging sort profit
    bool operator<(const Order& other) const {
        return totalProfit > other.totalProfit;
    }
};

#endif
