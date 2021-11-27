#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"
#include <iostream>
using namespace std;

WATCardOffice::WATCardOffice( Printer & prt, /*Bank & bank, TODO*/ unsigned int numCouriers ):prt{prt}, numCouriers{numCouriers}{

}

void WATCardOffice::main(){
    prt.print(Printer::Kind::WATCardOffice, 'S');
    // TODO
    prt.print(Printer::Kind::WATCardOffice, 'F');
}

