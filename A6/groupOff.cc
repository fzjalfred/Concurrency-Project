#include "groupOff.h"
#include "printer.h"
#include "WATCard.h"
#include <utility>

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
prt{prt},numStudents{numStudents},sodaCost{sodaCost},groupoffDelay{groupoffDelay},curCompletedStudents{0},curWaitingStudents{numStudents}, curCalledStudents{0}{
    cards = new WATCard::FWATCard[numStudents];
}   // Groupoff

Groupoff::~Groupoff(){
    for (unsigned int i = 0; i < numStudents; ++i){
        if (cards[i].available()){          // if read card is not used, delete it
             delete cards[i]();    
        }   // if
    }   // for
    delete[] cards;
}   // ~Groupoff

WATCard::FWATCard Groupoff::giftCard(){
    return cards[curCalledStudents];
}

void Groupoff::main(){
    PRINT(Printer::Groupoff, 'S');
    for (uint i = 0; i < numStudents; ++i){ // first wait for students to call
        _Accept(giftCard);
        curCalledStudents++;
    }
    for (;;){
      if (curCompletedStudents == numStudents) break;
        _Accept(~Groupoff){break;}
        _Else{    // Will not block at detor
            yield(groupoffDelay);   // yield before delivery
            WATCard* realCard = new WATCard();  // new watcard
            unsigned int rdmID = rng(curWaitingStudents-1); // generate a random number
            realCard->deposit(sodaCost);    
            cards[rdmID].delivery(realCard);   
            PRINT(Printer::Groupoff, 'D', sodaCost);
            std::swap(cards[rdmID], cards[curWaitingStudents-1]);   // move delivered card to the last
            curCompletedStudents++;
            curWaitingStudents--;
        }   //Else
    } // for
    PRINT(Printer::Groupoff, 'F');
}  // main

