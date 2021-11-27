#include "groupOff.h"
#include "printer.h"
#include "WATCard.h"
#include <utility>
#include <iostream>
using namespace std;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
prt{prt},numStudents{numStudents},sodaCost{sodaCost},groupoffDelay{groupoffDelay},curCompletedStudents{0},curWaitingStudents{numStudents}, curCalledStudents{0}{
    cards = new WATCard::FWATCard*[numStudents];
    realCards = new WATCard*[numStudents];
}

Groupoff::~Groupoff(){
    for (unsigned int i = 0; i < numStudents; ++i){
        delete *cards[i];   // delete real WATCard
        delete cards[i];    // delete future card
    }
    delete[] cards;
    delete[] realCards;
}

WATCard::FWATCard Groupoff::giftCard(){
    cards[curCalledStudents] = new WATCard::FWATCard;
    return *cards[curCalledStudents];
}

void Groupoff::main(){
    prt.print(Printer::Groupoff, 'S');
    for (;;){
        if (curCompletedStudents == numStudents) break;
        _Accept(~Groupoff){break;}
        or _Accept(giftCard){
            curCalledStudents++;
        } _Else{    // Will not block at detor
            if (curCalledStudents == numStudents){ // when each student obtains a future card, then
                WATCard* realCard = new WATCard();  // new watcard
                unsigned int rdmID = rng(curWaitingStudents-1); // generate a random number
                realCards[rdmID] = realCard;    // record that real card for delete
                realCard->deposit(sodaCost);    
                yield(groupoffDelay);   // yield before delivery
                cards[rdmID]->delivery(realCard);   
                prt.print(Printer::Groupoff, 'D', sodaCost);
                std::swap(cards[rdmID], cards[curWaitingStudents-1]);   // remove delivered card to the last
                curCompletedStudents++;
                curWaitingStudents--;
            }
        }
    }
    prt.print(Printer::Groupoff, 'F');
}

