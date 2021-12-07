#include "parent.h"
#include "MPRNG.h"

extern MPRNG rng;


Parent::Parent( Printer& prt, Bank& bank, 
unsigned int numStudents, unsigned int parentalDelay)
: prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay)
{}

Parent::~Parent() {}

void Parent::main() {
    PRINT(Printer::Kind::Parent, 'S');
    for (;;) {
        _Accept( ~Parent ) {
            break;
        } _Else;

        yield(parentalDelay);   // wait 
        uint parent_give = rng(1,3);
        uint sid = rng(0, numStudents - 1);
        PRINT(Printer::Kind::Parent, 'D', sid, parent_give);
        bank.deposit(sid, parent_give);
        
    }
    PRINT(Printer::Kind::Parent, 'F');
}