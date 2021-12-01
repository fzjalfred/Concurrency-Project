#include "groupOff.h"
#include "printer.h"
#include "WATCard.h"
#include <utility>

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
prt{prt},numStudents{numStudents},sodaCost{sodaCost},groupoffDelay{groupoffDelay},curCompletedStudents{0},curWaitingStudents{numStudents}, curCalledStudents{0}{
    cards = new WATCard::FWATCard[numStudents];
}

Groupoff::~Groupoff(){
    for (unsigned int i = 0; i < numStudents; ++i){
        if (cards[i].available()){      
             delete cards[i]();    
        }
    }
    delete[] cards;
}

WATCard::FWATCard Groupoff::giftCard(){
    return cards[curCalledStudents];
}

void Groupoff::main(){
    PRINT(Printer::Groupoff, 'S');
    for (;;){
        if (curCompletedStudents == numStudents) break;
        _Accept(~Groupoff){break;}
        or _Accept(giftCard){
            curCalledStudents++;
        } _Else{    // Will not block at detor
            if (curCalledStudents == numStudents){ // when each student obtains a future card, then
                WATCard* realCard = new WATCard();  // new watcard
                unsigned int rdmID = rng(curWaitingStudents-1); // generate a random number
                realCard->deposit(sodaCost);    
                yield(groupoffDelay);   // yield before delivery
                cards[rdmID].delivery(realCard);   
                PRINT(Printer::Groupoff, 'D', sodaCost);
                std::swap(cards[rdmID], cards[curWaitingStudents-1]);   // remove delivered card to the last
                curCompletedStudents++;
                curWaitingStudents--;
            }
        }
    }
    PRINT(Printer::Groupoff, 'F');
}

