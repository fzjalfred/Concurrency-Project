#include "truck.h"
#include "MPRNG.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include <algorithm>
extern MPRNG rng;


Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
    : prt(prt), nameServer(nameServer), plant(plant), 
    numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {
    }


Truck::~Truck() {}

/* compute the remain of the stock, sodaList, totalAmount, and totalLack*/
void Truck::computeRemain(uint* stock, uint* sodaList, uint& totalLack, uint& totalAmount){
    for (uint i = 0; i < 4; ++i){
        uint amount = std::min(sodaList[i], maxStockPerFlavour - stock[i]); 
        totalAmount -= amount;
        sodaList[i] -= amount;
        stock[i] += amount;
        totalLack += maxStockPerFlavour - stock[i];
    }
}

void Truck::main() {
    prt.print(Printer::Kind::Truck,'S');
    VendingMachine **mch_list =  nameServer.getMachineList();
    uint i = 0; // last machine idx
    for (;;){
        yield(rng(1,10));   // wait before each getShipment
        try{
            plant.getShipment(sodaList); // need to release when ending.
            uint total_amount = 0;
            for (int c = 0; c<4; c++){
                total_amount += sodaList[c];
            }   // for
            prt.print(Printer::Kind::Truck,'P', total_amount);
            uint times = numVendingMachines;
            for (;times > 0;i++) {
                if (total_amount == 0) break; // if no stock left, begin another ship
                if (i == numVendingMachines) i = 0; // begin another cycle
                VendingMachine* machine = mch_list[i];
                unsigned int *stock = machine->inventory();
                prt.print(Printer::Kind::Truck,'d', i, total_amount);

                uint total_lack = 0;    // total number of bottles not replenished
                computeRemain(stock, sodaList, total_lack, total_amount );  // main computation

                if (total_lack > 0) prt.print(Printer::Kind::Truck,'U', i, total_lack); // print iff lack > 0

                prt.print(Printer::Kind::Truck,'D', i, total_amount);
                machine->restocked(); // restock is completed.
                
                if (rng()%100 == 0) { // fix flat tire
                    prt.print(Printer::Kind::Truck,'X');
                    yield(10);
                }   // if
                times--;
            } // for
        }   catch(BottlingPlant::Shutdown &){break;}    // try
    }  
    prt.print(Printer::Kind::Truck,'F');
}