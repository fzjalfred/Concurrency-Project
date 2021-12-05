#include "bottlingPlant.h"
#include "MPRNG.h"
#include <iostream>

extern MPRNG rng;


BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments ):
    prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
    maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
    timeBetweenShipments(timeBetweenShipments), truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour) {
    }

BottlingPlant::~BottlingPlant() {}

void BottlingPlant::getShipment(unsigned int cargo[ ]) {
    if (is_shutdown) {
        throw BottlingPlant::Shutdown{};
    }
    for (int c = 0; c<4; c++){
        cargo[c] = production[c];
    }

}

void BottlingPlant::main() {
    PRINT(Printer::Kind::BottlingPlant,'S');
    try {
    for(;;) {   
        //perform a production run
        uint b = 0;
        for (int c = 0; c<4; c++) {
            int bottles = rng(0, maxShippedPerFlavour);
            production[c] = bottles;
            b += bottles;
        }
        PRINT(Printer::Kind::BottlingPlant,'G', b);
        yield(timeBetweenShipments);
        _Accept(~BottlingPlant) {
            is_shutdown = true;
            _Accept(getShipment);
        }
        or _Accept(getShipment) {
            PRINT(Printer::Kind::BottlingPlant,'P');
        }_Else;
        
    }
    } catch (uMutexFailure::RendezvousFailure &) {
        //std::cout<<"BottlingPlant is ending ..."<<std::endl;
    }
    PRINT(Printer::Kind::BottlingPlant,'F');
}