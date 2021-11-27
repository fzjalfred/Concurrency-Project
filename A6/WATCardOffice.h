#pragma once
#include "MPRNG.h"

extern MPRNG rng;
_Monitor Printer;

_Task WATCardOffice {
    /* TODO
	struct Job {							// marshalled arguments and return future
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};
	_Task Courier { ... };					// communicates with bank
    */ 
    Printer& prt;
    unsigned int numCouriers;
    unsigned int curAmount;
    unsigned int curSid;
	void main();
  public:
	_Event Lost {};							// lost WATCard
	WATCardOffice( Printer & prt, /*Bank & bank, TODO*/ unsigned int numCouriers );
	//WATCard::FWATCard create( unsigned int sid, unsigned int amount );
	//WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
	// Job * requestWork(); TODO
};