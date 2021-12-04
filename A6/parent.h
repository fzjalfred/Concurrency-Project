#pragma once
#include "printer.h"
#include "bank.h"

_Task Parent {
    Printer& prt;                   // printer
    Bank& bank;                         // bank 
    unsigned int numStudents;           // number of students
    unsigned int parentalDelay;         // parent delay

    void main();
    public:
    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};
