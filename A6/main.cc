#include <iostream>
#include "config.h"
#include <cstring>										// access: strcmp
#include <unistd.h>										// access: getpid
#include "printer.h"
#include "MPRNG.h"
#include "vendingMachine.h"
#include "nameServer.h"
#include "student.h"
#include "groupOff.h"
#include "WATCardOffice.h"
#include "bank.h"
#include "parent.h"
using namespace std;

MPRNG rng;

int main(int argc, char const *argv[])
{
    ConfigParms cparms;   // program configuration
    int processors = 1;    // num of processors
    int seed = getpid();    // default random seed
    try{
        switch (argc)
            {
                case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
                    processors = stoi( argv[3] ); if ( processors <= 0 ) throw 1;                
                }
                case 3: if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
                    seed = stoi( argv[2] ); if ( seed <= 0 ) throw 1;                
                }
                case 2: if ( strcmp( argv[1], "d" ) != 0 ) {	// default ?
                    processConfigFile( argv[1], cparms);  
                    break;                 
                }
                case 1: {
                    processConfigFile( "soda.config", cparms);  // parse default conf file 
                    break;
                }
                default:
                    throw 1;
            }   // switch
    }   catch(...){
        cerr << "Usage: " << argv[0] << "soda [ config-file | ’d’ [ seed (> 0) | ’d’ [ processors (> 0) | ’d’ ] ] ]" << endl;
        exit(EXIT_FAILURE);
    }   // try catch

    uProcessor p[processors - 1]; // number of kernel threads
    rng.set_seed(seed); // set rng's seed
    //if ( processors == 1 ) uThisProcessor().setPreemption( 0 ); // turn off time-slicing for reproducibility

    /* initialize all tasks */
    {
        Printer prt(cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers);
        Bank bank(cparms.numStudents);
        NameServer nameServer(prt,cparms.numVendingMachines, cparms.numStudents);
        Groupoff groupOff(prt, cparms.numStudents, cparms.sodaCost, cparms.groupoffDelay);
        WATCardOffice office(prt, bank, cparms.numCouriers);
        VendingMachine * machines[cparms.numVendingMachines];
        Student * students[cparms.numStudents];
        Parent parent(prt, bank, cparms.numStudents, cparms.parentalDelay);
        
        /* initialize all vending machines */
        for (unsigned int i = 0; i < cparms.numVendingMachines; ++i){
            machines[i] = new VendingMachine(prt, nameServer, i, cparms.sodaCost);
        }

        /* init all students */
        for (unsigned int i = 0; i < cparms.numStudents; ++i){
            students[i] = new Student(prt, nameServer, office, groupOff, i, cparms.maxPurchases);
        }

        /* wait all tasks to finish */
        for (unsigned int i = 0; i < cparms.numStudents; ++i){
            delete students[i];
        }

        /* wait all tasks to finish */
        for (unsigned int i = 0; i < cparms.numVendingMachines; ++i){
            delete machines[i];
        }
    }
    

    return 0;
}
