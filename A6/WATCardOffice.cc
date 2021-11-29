#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"

WATCardOffice::WATCardOffice( Printer & prt, /*Bank & bank, TODO*/ unsigned int numCouriers ):prt{prt}, numCouriers{numCouriers}{

}


WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
    curSid = sid;
    curAmount = amount;
    requests.push(WATCardOffice::Job(sid));
    return requests.back().result;
}

/*
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ){

}
*/
WATCardOffice::~WATCardOffice(){
    while(!requests.empty()){
        requests.pop();
    }
}

void WATCardOffice::main(){
    prt.print(Printer::Kind::WATCardOffice, 'S');
    // TODO
    
    for (;;){
        _Accept(~WATCardOffice){
            break;
        }   or _Accept(create){ // TODO Modify this when done courior
            yield(10);
            WATCard *realCard = new WATCard();
            realCard->deposit(curAmount);
            requests.back().result.delivery(realCard);
            prt.print(Printer::Kind::WATCardOffice, 'C', curSid, curAmount);
        }
    }
    prt.print(Printer::Kind::WATCardOffice, 'F');
}

