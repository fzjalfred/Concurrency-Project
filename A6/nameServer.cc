#include "nameServer.h"
#include "printer.h"
#include "vendingMachine.h"
#include <iostream>
using namespace std;
NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ): 
prt{prt}, numVendingMachines{numVendingMachines}, numStudents{numStudents}, registeredMach{0}{

    machList = new VendingMachine* [numVendingMachines];
    stuToMach = new unsigned int[numStudents];
    for (unsigned int i = 0; i < numStudents; ++i){
        stuToMach[i] = i % numVendingMachines;
    }   // for
}   // NameServer

void NameServer::VMregister( VendingMachine * vendingmachine ){
    currMachId = vendingmachine->getId();
    machList[currMachId] = vendingmachine;
}   // VMregister

NameServer::~NameServer(){
    delete [] machList;
    delete [] stuToMach;
}   // ~NameServer

VendingMachine ** NameServer::getMachineList(){
    return machList;
}   // getMachineList

VendingMachine * NameServer::getMachine( unsigned int id ){
    currStuId = id;
    currMachId = stuToMach[id];
    return machList[stuToMach[id]];
}   // getMachine

void NameServer::main(){
    prt.print(Printer::Kind::NameServer, 'S');
    for (;;){
        _Accept(~NameServer) {  // stop tsak
            prt.print(Printer::Kind::NameServer, 'F');
            break;
        } or _When (registeredMach < numVendingMachines) _Accept(VMregister){   // else register machine
            prt.print(Printer::Kind::NameServer, 'R', currMachId);
            registeredMach++;
        }or _When (registeredMach == numVendingMachines) _Accept(getMachine){   // when all machines are registered, accept getMachine
            prt.print(Printer::Kind::NameServer, 'N', currStuId, currMachId);
            stuToMach[currStuId] = (currMachId + 1) % numVendingMachines;
        }  or _When (registeredMach == numVendingMachines) _Accept(getMachineList){}
    }
}