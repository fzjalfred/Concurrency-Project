#include "WATCardOffice.h"
#include "WATCard.h"
#include "printer.h"
#include <iostream>

Job * WATCardOffice::requestWork() {
    if (requests.empty()) return nullptr;   // for sync
    auto job = requests.front();
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
        
        prt.print(Printer::Kind::Courier, id, 'S');
        for (;;) {
            auto job = cardOffice->requestWork();   // job to proceed
            if ( job == nullptr ) break;    // this means office ends
            prt.print(Printer::Kind::Courier, id, 't', job->sid, job->amount);
            bank.withdraw(job->sid, job->amount);
            job->card->deposit(job->amount);
            if ( rng(5) == 0 ) {  // card lost
                job->result.exception( new WATCardOffice::Lost );
                delete job->card;
                // print lost
                prt.print( Printer::Kind::Courier, id, 'L', job->sid ); 
            } else {
                job->result.delivery( job->card );
                // print delievery
                prt.print( Printer::Kind::Courier, id, 'T', 
                                job->sid, job->amount );
            }
            delete job;
        }

        prt.print(Printer::Kind::Courier, id, 'F');
        
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
    requests.push(new Job(sid, amount, new WATCard()));
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
    prt.print(Printer::Kind::WATCardOffice, 'S');
    
    for ( ; ; ) {
        _Accept ( ~WATCardOffice ) {
            break;
        } or _Accept ( create) {
            uint sid = requests.back()->sid;
            uint amount = requests.back()->amount;
            prt.print(Printer::Kind::WATCardOffice,'C', sid, amount);
        } or _Accept ( transfer) {
            uint sid = requests.back()->sid;
            uint amount = requests.back()->amount;
            prt.print(Printer::Kind::WATCardOffice,'T', sid, amount);
        } or _When( requests.size() ) _Accept ( requestWork ) { // block for a student's request
            requests.pop(); // the job is to be finished in the future.
            prt.print(Printer::Kind::WATCardOffice, 'W');
        } 
    } // for 
    
    // release the last call of requestWork
    for (unsigned int i = 0; i < numCouriers; i++) {
        _Accept ( requestWork );
    }
    prt.print(Printer::Kind::WATCardOffice, 'F');
}

