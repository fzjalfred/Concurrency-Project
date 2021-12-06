#include "WATCard.h"
#include "vendingMachine.h"
#include "printer.h"
#include "nameServer.h"
#include <iostream>

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ):
prt{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}{
    for (unsigned int i = 0; i < 4; ++i) stock[i] = 0;    //Remove it when done truck/plant
}

void VendingMachine::buy( VendingMachine::Flavours flavour, WATCard & card ){
    if (card.getBalance() < sodaCost){  // no sufficient fund
        flag = VendingMachine::Status::funds;
        _Throw VendingMachine::Funds{};
    }
    if (stock[flavour] == 0){   // no sufficient stock
        flag = VendingMachine::Status::stocks;
        _Throw VendingMachine::Stock{};
    }
    currCard = &card;
    currFlavour = flavour;
    if (rng() % 4 == 0){    // free soda
        flag = VendingMachine::Status::free;
        _Throw VendingMachine::Free{};
    }  else {
        waiting.wait();
    }
}

unsigned int * VendingMachine::inventory(){
    return stock;
}

void VendingMachine::restocked(){
    // indicates that restock is complete.
    // Nothing to do.
}

_Nomutex unsigned int VendingMachine::cost() const{
    return sodaCost;
}
_Nomutex unsigned int VendingMachine::getId() const{
    return id;
}

void VendingMachine::main(){
    
    nameServer.VMregister(this);
    PRINT(Printer::Kind::Vending, id,'S', sodaCost);
    for (;;){
        try{
            _Accept(~VendingMachine){break;}
            or _Accept(buy){ // normal buy
                currCard->withdraw(sodaCost);
                stock[currFlavour]--;
                PRINT(Printer::Kind::Vending, id, 'B', currFlavour, stock[currFlavour]);
                while (!waiting.empty()) waiting.signalBlock();
            }   // buy
            or  _Accept(inventory) {
                PRINT(Printer::Kind::Vending, id, 'r');
                    _Accept(restocked) {
                        PRINT(Printer::Kind::Vending, id, 'R');
                }
            } // block for restock. //OUPUT fix TODO
        }   catch(uMutexFailure::RendezvousFailure &){
            switch (flag)
            {
            case VendingMachine::Status::free:
                PRINT(Printer::Kind::Vending, id, 'A');
                stock[currFlavour]--;
                PRINT(Printer::Kind::Vending, id, 'B', currFlavour, stock[currFlavour]);
                break;
            case VendingMachine::Status::funds:
                break;
            case VendingMachine::Status::stocks:
                _Accept(inventory) {
                    PRINT(Printer::Kind::Vending, id, 'r');
                    _Accept(restocked) {
                        PRINT(Printer::Kind::Vending, id, 'R');
                    }
                } // block for restock.
                break;
            default:
                break;
            }   // switch
        } _Finally {
        }   // try     
    }   // for
    
    _Accept(inventory) { // release truck.
        PRINT(Printer::Kind::Vending, id, 'r');
        _Accept(restocked) {
                PRINT(Printer::Kind::Vending, id, 'R');
        }
    }
    PRINT(Printer::Kind::Vending, id, 'F');
}