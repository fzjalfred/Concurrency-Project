#pragma once
#include "printer.h"
#include "nameServer.h"

_Task BottlingPlant;

_Task Truck {
    Printer & prt;
    NameServer& nameServer; 
    BottlingPlant & plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;

    uint sodaList[4] = {0};
    void main();
    void computeRemain(uint* stock, uint* sodaList, uint& totalLack, uint& totalAmount);    // compute the remain of the stock, sodaList, totalAmount, and totalLack
    public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};
