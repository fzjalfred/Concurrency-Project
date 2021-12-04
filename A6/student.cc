#include "student.h"
#include "nameServer.h"
#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"
#include "groupOff.h"
#include "vendingMachine.h"


Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,unsigned int id, unsigned int maxPurchases )
:prt{prt}, nameServer{nameServer}, cardOffice{cardOffice}, groupoff{groupoff}, id{id}, maxPurchases{maxPurchases}, numPurchased{0}{}

void Student::main(){   
    //TODO Now watcard has infty amount
    unsigned int sodaToBuy = rng(1, maxPurchases);  // number of soda to buy
    VendingMachine::Flavours flavour = static_cast<VendingMachine::Flavours>(rng(3));   // flavour of soda to buy
    PRINT(Printer::Kind::Student,id,'S',flavour, sodaToBuy);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard* card = nullptr;                            // which card to use
    // obtain the location of vending machine
    VendingMachine* machine = nameServer.getMachine(id);
    PRINT(Printer::Kind::Student,id,'V', machine->getId());

    for (;;){
        if (numPurchased == sodaToBuy) break;
        yield(rng(1,10)); // yield 1-10 times
        _Select(watCard || giftCard);
    
        if (giftCard.available()){  // if giftcard is aviliable, first use giftcard
            try{
                card = giftCard();  // retrieve real card
                machine->buy(flavour, *card);
                PRINT(Printer::Kind::Student,id,'G', flavour, card->getBalance());
                giftCard.reset();   // prevent future usage
                numPurchased++;
                delete card;
            }   catch(VendingMachine::Free&){
                yield(4);
                PRINT(Printer::Kind::Student,id,'a', flavour, card->getBalance());
                numPurchased++;
            }   catch(VendingMachine::Stock&){
                
            }
        }   else{   // watcard is aviliable
            REBUY:
            try{
                card = watCard();
                machine->buy(flavour, *card);
                PRINT(Printer::Kind::Student,id,'B', flavour, card->getBalance()); 
                numPurchased++;
            }   catch(VendingMachine::Free&){
                yield(4);   // watch adv
                PRINT(Printer::Kind::Student,id,'A', flavour, card->getBalance());
                numPurchased++;
            }   catch(WATCardOffice::Lost &e){
                watCard = cardOffice.create(id, 5);
                goto REBUY;
            }   catch(VendingMachine::Funds&){
                watCard = cardOffice.transfer(id, machine->cost() + 5, card);
                goto REBUY;
            }   catch(VendingMachine::Stock&){
                goto REBUY;
            }
        }   // if
    } // for
    try {
        card = watCard();   // delete watcard
        delete card;
    } catch (WATCardOffice::Lost &) {

    }
    

    PRINT(Printer::Kind::Student,id,'F');
}