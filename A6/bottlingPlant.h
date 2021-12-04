#pragma once
#include "printer.h"
#include "nameServer.h"

_Task BottlingPlant {
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;



    void main();
    public:
    _Event Shutdown {}; // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[ ] );
};