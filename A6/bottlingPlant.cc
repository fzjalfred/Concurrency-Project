#include "bottlingPlant.h"


BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments ):
    prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
    maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
    timeBetweenShipments(timeBetweenShipments) {}

BottlingPlant::~BottlingPlant() {}

void BottlingPlant::getShipment(unsigned int cargo[ ]) {
    (void*) cargo;
}

void BottlingPlant::main() {
    
}