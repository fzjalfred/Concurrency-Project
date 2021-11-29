#pragma once
#include "MPRNG.h"

extern MPRNG rng;

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;

_Task Student {
    Printer & prt;
    NameServer & nameServer;
    WATCardOffice & cardOffice;
    Groupoff & groupoff;
    unsigned int id;
    unsigned int maxPurchases;  
    unsigned int numPurchased;  // number of soda have brought
    void main();

  public:
    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,unsigned int id, unsigned int maxPurchases );
};