//DECLARATION
//I hold a copy of this assignment that I can produce if the original is lost or damaged.
//I hereby certify that no part of this assignment has been copied from any other student’s work or
//from any other source except where due acknowledgement is made in the assignment. No part
//of this assignment has been written/produced for me by another person except where such
//collaboration has been authorised by the subject lecturer concerned.
//Iused Vs code to make the entire assignemnt cuz of amazing debugging libs then ran and tested it on Clion.
#include <iostream>
#include <string>
#include <exception>   
using namespace std;

#include "Constant.h"
#include "Unit.h"
#include "Component.h"
#include "Product.h"
#include "Order.h"
#include "Schedule.h"
#include "BundleGenerator.h"
#include "SmartSchedule.h"

void runTask1() {
    cout << "\n=== Task 1: unit,component & product ===\n";
    Component c0(0);
    cout << "Component: " << c0.getName() << " | Cost = $" << c0.getCost() << "\n";

    Product p7(7);
    cout << p7.brief() << "\n";
}

void runTask2() {
    cout << "\n=== Task 2: order ===\n";
    Order o1(101, 7, 19, 45);
    Order o2(102, 0, 10, 50);
    Order o3(103, 15, 5, 60);

    o1.display();
    o2.display();
    o3.display();
}

void runTask3() {
    cout << "\n=== task 3: schedule ===\n";
    Schedule sch("OrderBundle.txt");

    if (!sch.loadOrders()) {
        cerr << "Failed to open bunle file.\n";
        return;
    }

    sch.runSchedule();
    sch.printReports("Schedule");
    cout << "Reports generated in current folder.\n";
}

void runTask4() {
    cout << "\n=== Task 4: bundle generator ===\n";

    try {
        BundleGenerator gen;
        gen.generateBundle("OrderBundle.txt");   // random ord
        cout << "Generated random OrderBundle.txt succesfully!\n";
    } catch (const exception& e) {
        cerr << "Error generating bundle: " << e.what() << "\n";
        return;
    }

    cout << "\nRunning task 3 automatically with new bundle...\n";
    runTask3();
}

void runTask5() {
    cout << "\n=== task 5: smartSchedule optimization ===\n";
    SmartSchedule smart("OrderBundle.txt");

    if (!smart.loadOrders()) {
        cerr << "cant not open OrderBundle.txt.\n";
        return;
    }

    smart.optimizeSchedule();
    cout << "Smart schedule made open SmartSchedule_Report.txt for details.\n";
}

int main() {
    // keep stdout flushed so the menu shows before input
    // ios::sync_with_stdio(false);   // <- comment this out to avoid buffering issues
    cin.tie(nullptr);

    cout << "========== OOP Assignment 2 ==========\n";
    cout << "Select an optin to run:\n";
    cout << "1) Task 1 - unit,component & product test\n";
    cout << "2) Task 2 - order test\n";
    cout << "3) Task 3 - schedule\n";
    cout << "4) Task 4 - bundle generator + gchedule\n";
    cout << "5) Task 5 - smartSchedule optimization\n";
    cout << "======================================\n";
    cout << "Enter task: " << flush;   // force immediate display

    int choice;
    cin >> choice;
    cout << "\n";

    switch (choice) {
        case 1: runTask1(); break;
        case 2: runTask2(); break;
        case 3: runTask3(); break;
        case 4: runTask4(); break;
        case 5: runTask5(); break;
        default: cout << "Invalid option.\n"; break;
    }

    cout << "\n=========== END done! ===========\n";
    return 0;
}
