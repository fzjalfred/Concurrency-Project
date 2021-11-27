
#include "vendingMachine.h"
#include "printer.h"
#include "nameServer.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ):
prt{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}{}

void VendingMachine::buy( Flavours flavour/*, WATCard & card */){
    //TODO
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
    prt.print(Printer::Kind::Vending, id,'S');
    nameServer.VMregister(this);
    prt.print(Printer::Kind::Vending, id, 'F');
}