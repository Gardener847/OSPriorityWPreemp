#include "PrioWPreemp.h"
#include <iostream>

using namespace std;

/*	
	List of assumptions:
	1) there is always a set of incoming processes, so the algorithm must be dynamic
		a) an incrementor would be used to simulate an ongoing clock and, for each clock 
			tick (or increment), the algorithm would check for a process's arrival time
			using the incoming process's process control block (PCB)
	2) everytime a new process arrives, execution order is dynamically rearranged due 
	to the priority preemption and burst time, which is indicated by the process's (PCB)
		a) rearrangement would depend on arrival process' priority compared to process already
			in ready queue
	3) for better testing purposes, randomize PCB values
	4) everything will be tracked by using the systemStat struct
*/

int main() {
	cout << "Creating 4 processes for testing..." << endl;

	//declare incoming processes
	PCB proc1;
	PCB proc2;
	PCB proc3;
	PCB proc4;
	//instantiate process's PCB
	proc1.processNum = 1;
	proc2.processNum = 2;
	proc3.processNum = 3;
	proc4.processNum = 4;

	proc1.priority = 1;
	proc2.priority = 2;
	proc3.priority = 3;
	proc4.priority = 4;

	proc1.burstTime = 4;
	proc2.burstTime = 4;
	proc3.burstTime = 4;
	proc4.burstTime = 4;

	proc1.arrivalTime = 1;
	proc2.arrivalTime = 2;
	proc3.arrivalTime = 3;
	proc4.arrivalTime = 4;

	PrioWPreemp scheduler;
	scheduler.storeProcesses(proc1);
	scheduler.storeProcesses(proc2);
	scheduler.storeProcesses(proc3);
	scheduler.storeProcesses(proc4);

	int numProc;
	char response;
	//let user dynamically add processes
	cout << "\nDo you want to add a process, or processes? (Y/N): ";
	cin >> response;
	if (response == 'y' || response == 'Y') {
		while (response == 'y' || response == 'Y') {
			cout << "Input the number of processes you want to enter: ";
			cin >> numProc;
			PCB *procs = new PCB[numProc];
			for (int i = 0; i < numProc; i++) {
				cout << "\nEnter the following information for process " << i + 1 << " using positive whole number higher than 0";
				cout << "\nProcess number greater than 4 and must be unique: ";
				cin >> procs[i].processNum;
				cout << "\nPriority: ";
				cin >> procs[i].priority;
				cout << "\nBurst time: ";
				cin >> procs[i].burstTime;
				cout << "\nArrival time: ";
				cin >> procs[i].arrivalTime;
			}
			scheduler.storeProcesses(procs, numProc);
			
			cout << "Do you want to add a process, or processes? (Y/N): ";
			cin >> response;
			if (response != 'y' || response != 'Y')
				cout << "\nTerminating the program after displaying Gantt Chart..." << endl;
		}
	}
	else 
		cout << "\nTerminating the program after displaying Gantt Chart..." << endl;

	scheduler.schedule();
	scheduler.displayGantt();

	cout << "Did it just run?!?!?!?" << endl;
	cin >> response;

	return 0;
}
