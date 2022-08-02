#include "WATCard.h"
#include "vendingMachine.h"
#include "printer.h"
#include "nameServer.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ):
prt{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}{
    
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
    currCard = &card;   // store info and perform computation in main
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
    prt.print(Printer::Kind::Vending, id,'S', sodaCost);
    nameServer.VMregister(this);
    FOR: for (;;){
        try{
            _Accept(~VendingMachine){
                break;
            }
            or _Accept(buy){ // normal buy
                currCard->withdraw(sodaCost);
                stock[currFlavour]--;
                prt.print(Printer::Kind::Vending, id, 'B', currFlavour, stock[currFlavour]);
                waiting.signalBlock();
            }   // buy
            or  _Accept(inventory) {
                
                prt.print(Printer::Kind::Vending, id, 'r');
                    _Accept(restocked) {
                        prt.print(Printer::Kind::Vending, id, 'R');
                } 
            } // block for restock. //OUPUT fix TODO
        }   catch(uMutexFailure::RendezvousFailure &){
            switch (flag)
            {
            case VendingMachine::Status::free:
                prt.print(Printer::Kind::Vending, id, 'A');
                stock[currFlavour]--;
                break;
            case VendingMachine::Status::funds:
                break;
            case VendingMachine::Status::stocks:
                _Accept(inventory) {
                    prt.print(Printer::Kind::Vending, id, 'r');
                    _Accept(restocked) {
                        prt.print(Printer::Kind::Vending, id, 'R');
                    }  
                } // block for restock.
                or _Accept(~VendingMachine){
                        break FOR;
                }   // handle the case when truck is destroyed
                break;
            default:
                break;
            }   // switch
        } // try     
    }   // for
    
    prt.print(Printer::Kind::Vending, id, 'F');
}