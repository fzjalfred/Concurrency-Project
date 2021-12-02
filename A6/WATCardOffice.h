#pragma once
#include "MPRNG.h"
#include "bank.h"
#include "WATCard.h"
#include <queue>

extern MPRNG rng;
_Monitor Printer;

struct Job {							// marshalled arguments and return future TODO
		unsigned int sid;					// call arguments (YOU DEFINE "Args")
		uint amount;
		WATCard::FWATCard result;			// return future
		Job( unsigned int sid, uint amount ) : sid( sid ), amount(amount) {}
};

_Task WATCardOffice {

	
	_Task Courier;					// communicates with bank TODO

    //WATCard::FWATCard card;
    std::queue<Job> requests;
    Printer& prt;
    unsigned int numCouriers;
    unsigned int curAmount;
    unsigned int curSid;
	WATCard *realCard;
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