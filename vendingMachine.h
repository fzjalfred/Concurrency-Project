#pragma once
#include "MPRNG.h"

extern MPRNG rng;

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
	void main();
	Printer& prt;
	NameServer& nameServer;
    unsigned int id;
    unsigned int sodaCost;
	unsigned int stock[4] = {0, 0, 0, 0};	// stock for each flavor
	WATCard* currCard;
	uCondition waiting;

  public:
	enum Flavours { Orange, Cherry, LaoGanMa, Lemon }; 				// flavours of soda (YOU DEFINE)
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// flavour out of stock
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	void buy( Flavours flavour, WATCard & card);
	unsigned int * inventory();			// TODO
	void restocked();					// TODO
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;

  private:
	Flavours currFlavour;
	enum Status {free, funds, stocks};
	Status flag;
};
