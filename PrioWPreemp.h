#ifndef PRIOWPREEMP_H
#define PRIOWPREEMP_H
#include <vector>

using namespace std;

struct PCB {
	int processNum;
	int priority;
	int burstTime;
	int arrivalTime;
	int execTime;
	int lastExecTime;
	int waitingTime;
	bool wasPreempted;
	PCB();
};

struct SystemStat {
	PCB curProc;
	int curBurstTime;
	int time;
	SystemStat();
};

class PrioWPreemp {
private:
	SystemStat status;
	vector<PCB> readyQueue;
	vector<PCB> waitQueue;
	vector<PCB> execProc;
	vector<int> times;
	int totalWaitTime;
	int totalRunTime;

public:
	PrioWPreemp();
	void storeProcesses(PCB process);
	void storeProcesses(PCB *process, int numProc);
	void schedule();
	void runProc();
	void getAveWait();
	void displayGantt();

};
#endif // !PRIOWPREEMP_H
