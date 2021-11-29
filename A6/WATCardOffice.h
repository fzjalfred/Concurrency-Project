#pragma once
#include "MPRNG.h"
#include "WATCard.h"
#include <queue>

extern MPRNG rng;
_Monitor Printer;

_Task WATCardOffice {

	struct Job {							// marshalled arguments and return future TODO
		unsigned int sid;					// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( unsigned int sid ) : sid( sid ) {}
	};
	//_Task Courier { ... };					// communicates with bank TODO

    //WATCard::FWATCard card;
    std::queue<Job> requests;
    Printer& prt;
    unsigned int numCouriers;
    unsigned int curAmount;
    unsigned int curSid;
	void main();
  public:
	_Event Lost {};							// lost WATCard
    ~WATCardOffice();
	WATCardOffice( Printer & prt, /*Bank & bank, TODO*/ unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount );
	// WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
	// Job * requestWork(); TODO
};