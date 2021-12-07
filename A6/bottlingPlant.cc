#include "bottlingPlant.h"
#include "MPRNG.h"

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
        // wait for production
        yield(timeBetweenShipments);
        //perform a production run
        uint b = 0; // total num of production
        for (int c = 0; c<4; c++) {
            int bottles = rng(0, maxShippedPerFlavour);
            production[c] = bottles;
            b += bottles;
        }
        PRINT(Printer::Kind::BottlingPlant,'G', b);
        
        _Accept(~BottlingPlant) {
            is_shutdown = true;
            _Accept(getShipment);
        }
        or _Accept(getShipment) {   // wait for truck to pickup
            PRINT(Printer::Kind::BottlingPlant,'P');
        }
        
    }
    } catch (uMutexFailure::RendezvousFailure &) {}
    PRINT(Printer::Kind::BottlingPlant,'F');
}