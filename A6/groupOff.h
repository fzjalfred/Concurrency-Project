#pragma once
#include "WATCard.h"
#include "MPRNG.h"

extern MPRNG rng;
_Monitor Printer;

_Task Groupoff {
    WATCard::FWATCard *cards;
    Printer & prt;
    unsigned int numStudents;
    unsigned int sodaCost; 
    unsigned int groupoffDelay;
    unsigned int curCompletedStudents;  // number of students fully complete
    unsigned int curWaitingStudents;  // number of students waiting for realcard
    unsigned int curCalledStudents; // number of students called giftCard()
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
	WATCard::FWATCard giftCard();
};