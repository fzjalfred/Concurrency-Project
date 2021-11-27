#pragma once

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
	Printer& prt;	// printer
	unsigned int numVendingMachines;	// total num of mach
	unsigned int numStudents;	// total num of stu
	unsigned int registeredMach;	// current registered mach num
	VendingMachine ** machList;		// mach list
	unsigned int * stuToMach;				// student id - mach id mapping
	unsigned int currMachId;
	unsigned int currStuId;
	void main();
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	~NameServer();
	void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id );
	VendingMachine ** getMachineList();
};
