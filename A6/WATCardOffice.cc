#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"
#include <iostream>

Job * WATCardOffice::requestWork() {
    if (requests.empty()) {
        return nullptr;
    }
    auto job = requests.front();
    requests.pop(); // the job is to be finished in the future.
    PRINT(Printer::Kind::WATCardOffice, 'W');
    return job;
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
            bank.withdraw(job->sid, job->amount);
            job->card->deposit(job->amount);
            if ( rng(5) == 0 ) {  // card lost
                job->result.exception( new WATCardOffice::Lost );
                delete job->card;
                // print lost
                PRINT( Printer::Kind::Courier, id, 'L', job->sid ); 
            } else {
                job->result.delivery( job->card );
                // print delievery
                PRINT( Printer::Kind::Courier, id, 'T', 
                                job->sid, job->amount );
            }
            delete job;
        }

        PRINT(Printer::Kind::Courier, id, 'F');
        
    }
};


WATCardOffice::WATCardOffice( Printer & prt, Bank& bank, unsigned int numCouriers ):
prt{prt}, numCouriers{numCouriers}, bank{bank} {
    couriersPool = new Courier*[numCouriers];
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        couriersPool[i] = new Courier( prt, this, bank, i );
    }
}


WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
    realCard = new WATCard();
    requests.push(new Job(sid, amount, realCard));
    return requests.back()->result;
}


WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ){
    requests.push(new Job(sid, amount, card));
    return requests.back()->result;
}

WATCardOffice::~WATCardOffice(){
    while(!requests.empty()){
        delete requests.front();
        requests.pop();
    }
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        delete couriersPool[i];
    }
    delete [] couriersPool; 
}

void WATCardOffice::main(){
    PRINT(Printer::Kind::WATCardOffice, 'S');
    
    for ( ; ; ) {
        _Accept ( ~WATCardOffice ) {
            break;
        } or _Accept ( create, transfer ) {
            if (!waiting.empty()) {
                waiting.signal();
            }
        } or _When( requests.size() ) _Accept ( requestWork ) { // block for a student's request
            
        } 
    } // for 
    
    // release the last call of requestWork
    for (unsigned int i = 0; i < numCouriers; i++) {
        _Accept ( requestWork );
    }
    
    // for (;;){
    //     _Accept(~WATCardOffice){
    //         break;
    //     }   or _Accept(create){ // TODO Modify this when done courior
    //         yield(10);
    //         realCard = new WATCard();
    //         realCard->deposit(curAmount);
    //         requests.back().result.delivery(realCard);
    //         PRINT(Printer::Kind::WATCardOffice, 'C', curSid, curAmount);
    //     }   or _Accept(transfer) {
    //         realCard->deposit(curAmount);
    //         requests.back().result.delivery(realCard);
    //         PRINT(Printer::Kind::WATCardOffice, 'T', curSid, curAmount);
    //     }
    // }
    PRINT(Printer::Kind::WATCardOffice, 'F');
}

