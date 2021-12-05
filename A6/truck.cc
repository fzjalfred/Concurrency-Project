#include "truck.h"
#include "MPRNG.h"
#include "vendingMachine.h"
extern MPRNG rng;
#include <iostream>

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
    : prt(prt), nameServer(nameServer), plant(plant), 
    numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {
    }


Truck::~Truck() {}


void Truck::main() {
    PRINT(Printer::Kind::Truck,'S');
    VendingMachine **mch_list =  nameServer.getMachineList();
    for (int a = 0;a<20;a++) {
        _Accept(~Truck) {
            break;
        } _Else;

        yield(rng(1,10));
        //BottlingPlant::getShipment(sodaList);
        {
            sodaList[0] = 10;
            sodaList[1] = 10;
            sodaList[2] = 10;
            sodaList[3] = 10;
        }
        PRINT(Printer::Kind::Truck,'P', 4);
        
        for (uint i = 0; i<numVendingMachines; i++) {
            VendingMachine* machine = mch_list[i];
            unsigned int *stock = machine->inventory();
            uint total_remain = 0;
            for (int c = 0; c<4; c++) {
                total_remain+= stock[c];
            }
            PRINT(Printer::Kind::Truck,'d', i, total_remain);
            
            uint total_lack = 0;
            for (int c = 0; c<4; c++) {
                stock[c] += sodaList[c];
                if (stock[c]>maxStockPerFlavour) {
                    sodaList[c] = stock[c] - maxStockPerFlavour;
                    stock[c] = maxStockPerFlavour;
                } else {
                    total_lack+=maxStockPerFlavour - stock[c];
                    sodaList[c] = 0;
                }
            }
            
            PRINT(Printer::Kind::Truck,'U', i, total_lack);
            uint cargo_remain = 0;
            for (int c = 0; c<4; c++) {
                cargo_remain+= sodaList[c];
            }
            PRINT(Printer::Kind::Truck,'D', i, cargo_remain);
            machine->restocked(); // restock is completed.
            
            if (rng()%100 == 0) { // fix flat tire
                PRINT(Printer::Kind::Truck,'X');
                yield(10);
            }
        }
    }
    PRINT(Printer::Kind::Truck,'F');
}