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

    int sodaList[4] = {0};
    void main();
    public:
    Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};
