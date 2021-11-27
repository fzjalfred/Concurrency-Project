#include "student.h"
#include "nameServer.h"
#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"
#include "groupOff.h"


Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,unsigned int id, unsigned int maxPurchases ):prt{prt}, nameServer{nameServer}, cardOffice{cardOffice}, groupoff{groupoff}, id{id}, maxPurchases{maxPurchases}{}

void Student::main(){   //TODO
    prt.print(Printer::Kind::Student,id,'S');
    WATCard::FWATCard giftCard = groupoff.giftCard();
    WATCard* card = giftCard();
    prt.print(Printer::Kind::Student,id,'P', card->getBalance());
    prt.print(Printer::Kind::Student,id,'F');
}