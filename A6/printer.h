#pragma once

_Monitor Printer {
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
  private:
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    unsigned int size;
    unsigned int blockSize;
    char* buffer;
    void flush();   // flush the buffer
    void printBuffer(); // print the buffer
    void printIfOverWrite(unsigned int posn);  // if the specificied id is overwritten, print the buffer and flush the buffer
    unsigned int getPosn( Kind kind, unsigned int lid = 0 );  // get position
    void writeNum(unsigned int & posn, unsigned int num); // write number
  public:
    
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
      ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, unsigned int value1 ); 
    void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
    void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );

};