#include "WATCard.h"
#include "vendingMachine.h"
#include "printer.h"
#include "nameServer.h"


VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ):
prt{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}{
    for (unsigned int i = 0; i < 4; ++i) stock[i] = 999;    //Remove it when done truck/plant
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
    //TODO
}

_Nomutex unsigned int VendingMachine::cost() const{
    return sodaCost;
}
_Nomutex unsigned int VendingMachine::getId() const{
    return id;
}

void VendingMachine::main(){
    PRINT(Printer::Kind::Vending, id,'S');
    nameServer.VMregister(this);
    for (;;){
        try{
            _Accept(~VendingMachine){break;}
            or _Accept(buy){ // normal buy
                currCard->withdraw(sodaCost);
                stock[currFlavour]--;
                PRINT(Printer::Kind::Vending, id, 'B', currFlavour, stock[currFlavour]);
                while (!waiting.empty()) waiting.signalBlock();
            }   // buy
        }   catch(uMutexFailure::RendezvousFailure &){  // exp buy TODO for restocking and funding
            switch (flag)
            {
            case VendingMachine::Status::free:
                PRINT(Printer::Kind::Vending, id, 'A');
                stock[currFlavour]--;
                PRINT(Printer::Kind::Vending, id, 'B', currFlavour, stock[currFlavour]);
                break;
            case VendingMachine::Status::funds:
                //TODO
                break;
            case VendingMachine::Status::stocks:
                //TODO
                break;
            default:
                break;
            }   // switch
        }   // try     
    }   // for  
    PRINT(Printer::Kind::Vending, id, 'F');
}