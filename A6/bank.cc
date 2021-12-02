#include "bank.h"

Bank::Bank( unsigned int numStudents ) {
    accounts = new uint [numStudents] {0};
}
Bank::~Bank(){
    delete [] accounts;
}
void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[id] += amount;
}
void Bank::withdraw( unsigned int id, unsigned int amount ) {
    for (;;) {
        if (accounts[id] >= amount) break;
        _Accept(deposit);
    }
    accounts[id] -= amount;
}