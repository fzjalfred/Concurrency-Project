#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"


Job * WATCardOffice::requestWork() {

}

_Task WATCardOffice::Courier {
    Printer& prt;
    WATCardOffice* cardOffice;
    Bank& bank;
    unsigned int id;
    public:
    Courier( Printer& prt, WATCardOffice* cardOffice, Bank& bank, unsigned int id ): 
            prt{prt}, cardOffice(cardOffice) ,bank{bank}, id{id} {} 
    private:
    void main() {
        PRINT(Printer::Kind::Courier, id, 'S');
        for (;;) {
            auto job = cardOffice->requestWork(); 
            if ( job == nullptr ) break;
            PRINT(Printer::Kind::Courier, id, 't', job->sid, job->amount);
        }
        
    }
};


WATCardOffice::WATCardOffice( Printer & prt, Bank& bank, unsigned int numCouriers ):
prt{prt}, numCouriers{numCouriers}, bank{bank} {

}


WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
    curSid = sid;
    curAmount = amount;
    requests.push(Job(sid, amount));
    return requests.back().result;
}


WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ){
    curSid = sid;
    curAmount = amount;
    realCard = card;
    requests.push(Job(sid, amount));
    return requests.back().result;
}

WATCardOffice::~WATCardOffice(){
    while(!requests.empty()){
        requests.pop();
    }
}

void WATCardOffice::main(){
    PRINT(Printer::Kind::WATCardOffice, 'S');
    // TODO
    
    for (;;){
        _Accept(~WATCardOffice){
            break;
        }   or _Accept(create){ // TODO Modify this when done courior
            yield(10);
            realCard = new WATCard();
            realCard->deposit(curAmount);
            requests.back().result.delivery(realCard);
            PRINT(Printer::Kind::WATCardOffice, 'C', curSid, curAmount);
        }   or _Accept(transfer) {
            realCard->deposit(curAmount);
            requests.back().result.delivery(realCard);
            PRINT(Printer::Kind::WATCardOffice, 'T', curSid, curAmount);
        }
    }
    PRINT(Printer::Kind::WATCardOffice, 'F');
}

