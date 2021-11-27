

#include "printer.h"
#include <algorithm>
#include <iostream>

using namespace std;



Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ):
    numStudents{numStudents}, numVendingMachines{numVendingMachines}, numCouriers{numCouriers}, size{8*(6 + numStudents + numVendingMachines + numCouriers)}, blockSize{8}{

    cout << "Parent" << '\t';
    cout << "Gropoff" << '\t';
    cout << "WATOff" << '\t';
    cout << "Names" << '\t';
    cout << "Truck" << '\t';
    cout << "Plant" << '\t';
    
    for (unsigned int i = 0; i < numStudents; ++i){ // output student header
        cout << "Stud" << i << '\t';
    }
    for (unsigned int i = 0; i < numVendingMachines; ++i){  // output machine header
        cout << "Mach" << i << '\t';
    }
    for (unsigned int i = 0; i < numCouriers; ++i){ // output courier header
        cout << "Cour" << i << '\t';
    }
    cout << endl;
    unsigned int numBlocks = 6 + numStudents + numVendingMachines + numCouriers;    // num of blocks need to print *******
    for (unsigned int i = 0; i < numBlocks; ++i){   // output block
        cout << "*******" << '\t';
    }
    cout << endl;
    buffer = new char [size];   // create buffer
    flush();    // clean the buffer
} // Printer 

Printer::~Printer(){
    printBuffer();
    cout << "*****************" << endl;

    delete[] buffer;
}   // ~Printer

void Printer::flush(){
    for (unsigned int i = 0; i < size; ++i){
        if (i % blockSize == 0) {   // set head of each block to '\t', others to 0
            buffer[i] = '\t';
        }   else {
            buffer[i] = 0;
        }   // if
    }   // for
}   // flush

void Printer::printBuffer(){
    for (unsigned int i = 0; i < size; ++i){
            if (buffer[i]) cout << buffer[i];
        }   // for
    cout << endl;
}   // printBuffer

void Printer::printIfOverWrite(unsigned int posn){
    if (buffer[posn] != '\t'){

        printBuffer();      

        flush();
    }   // if
    
}   // printIfOverWrite

unsigned int Printer::getPosn( Printer::Kind kind, unsigned int lid ){
    unsigned int res = 0;
    unsigned int offset = 6 * blockSize; // offset for student, mach, cour
    switch (kind)
    {
    case Printer::Kind::Student:    
        res = offset + lid * blockSize;
        break;
    case Printer::Kind::Vending:    
        res = offset + blockSize * numStudents + lid * blockSize;   // jump students' blocks
        break;
    case Printer::Kind::Courier:
        res = offset + blockSize * (numStudents + numVendingMachines) + lid * blockSize;    // jump students and machines blocks
        break;
    default:
        res = 8 * kind;
        break;
    }   // switch
    return res;
}   // getPosn

void Printer::writeNum(unsigned int & posn, unsigned int num){
    char space[8];  // stack allocated space to store num
    unsigned int idx = 0;   // idx of space
    if (num == 0) {
        space[idx] = '0';
        idx++;
    }   else {
        while (num > 0){
            space[idx] = '0' + num % 10;
            num /= 10;
            idx++;
        }   // while
    }   // if
    
    for (int i = idx-1; i >= 0; --i){
        buffer[posn++] = space[i];
    }   // for
}   // writeNum

void Printer::print( Printer::Kind kind, char state ){
    unsigned int posn = getPosn(kind);
    printIfOverWrite(posn);
    buffer[posn++] = state; 
    buffer[posn] = '\t';
}   // print

void Printer::print( Printer::Kind kind, unsigned int lid, char state ){
    unsigned int posn = getPosn(kind, lid);
    printIfOverWrite(posn);
    buffer[posn++] = state;
    buffer[posn] = '\t';
}   // print

void Printer::print( Printer::Kind kind, char state, unsigned int value1 ){
    unsigned int posn = getPosn(kind);
    printIfOverWrite(posn);
    buffer[posn++] = state;
    writeNum(posn, value1);
    buffer[posn] = '\t';
}   // print

void Printer::print( Printer::Kind kind, unsigned int lid, char state, unsigned int value1 ){
    unsigned int posn = getPosn(kind, lid);
    printIfOverWrite(posn);
    buffer[posn++] = state;
    writeNum(posn, value1);
    buffer[posn] = '\t';
}   // print

void Printer::print( Printer::Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ){
    unsigned int posn = getPosn(kind, lid);
    printIfOverWrite(posn);
    buffer[posn++] = state;
    writeNum(posn, value1);
    buffer[posn++] = ',';
    writeNum(posn, value2);
    buffer[posn] = '\t';
}   // print

void Printer::print( Printer::Kind kind, char state, unsigned int value1, unsigned int value2 ){
    unsigned int posn = getPosn(kind);
    printIfOverWrite(posn);
    buffer[posn++] = state;
    writeNum(posn, value1);
    buffer[posn++] = ',';
    writeNum(posn, value2);
    buffer[posn] = '\t';
}   // print