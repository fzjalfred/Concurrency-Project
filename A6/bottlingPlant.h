#pragma once
#include "printer.h"
#include "nameServer.h"
#include "truck.h"

_Task BottlingPlant {
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    Truck truck;
    bool is_shutdown = false;   // flag to determine when to raise exp on truck
    uint production[4] = {0};

    void main();
    public:
    _Event Shutdown {}; // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[ ] );
};