#include "parent.h"
#include "MPRNG.h"

extern MPRNG rng;


Parent::Parent( Printer& prt, Bank& bank, 
unsigned int numStudents, unsigned int parentalDelay)
: prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay)
{}

Parent::~Parent() {}

void Parent::main() {
    prt.print(Printer::Kind::Parent, 'S');
    for (;;) {
        _Accept( ~Parent ) {
            break;
        } _Else;

        yield(parentalDelay);   // wait 
        uint parent_give = rng(1,3);
        uint sid = rng(0, numStudents - 1);
        prt.print(Printer::Kind::Parent, 'D', sid, parent_give);
        bank.deposit(sid, parent_give);
        
    }
    prt.print(Printer::Kind::Parent, 'F');
}