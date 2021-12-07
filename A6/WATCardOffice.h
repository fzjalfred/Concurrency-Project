#pragma once
#include "MPRNG.h"
#include "bank.h"
#include "WATCard.h"
#include <queue>

extern MPRNG rng;
_Monitor Printer;

struct Job {							// marshalled arguments and return future TODO
		unsigned int sid;					// call arguments (YOU DEFINE "Args")
		uint amount;                     // amount to be transfered.
		WATCard* card;                  // student's watcard
		WATCard::FWATCard result;			// return future
		Job( unsigned int sid, uint amount, WATCard* card = nullptr ) : sid( sid ), amount(amount), card(card) {}
};

_Task WATCardOffice {

	
	_Task Courier;					// communicates with bank TODO
	Courier** couriersPool;             // courier pool
    std::queue<Job*> requests;		// job queue
    Printer& prt;
    unsigned int numCouriers;
	Bank& bank;
	void main();
  public:
	_Event Lost {};							// lost WATCard
    ~WATCardOffice();
	WATCardOffice( Printer & prt, Bank& bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount );
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
	Job * requestWork();
};