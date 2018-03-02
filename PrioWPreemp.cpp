#include "PrioWPreemp.h"
#include <iostream>
#include <vector>

using namespace std;

PCB::PCB() {
	waitingTime = 0;
	wasPreempted = false;
	execTime = 0;
}

SystemStat::SystemStat() {
	curProc.processNum = -1;
	time = 1;
}

PrioWPreemp::PrioWPreemp() {
	totalRunTime = 0;
	times.push_back(0);
}

void PrioWPreemp::storeProcesses(PCB process) {
	readyQueue.push_back(process);
	totalRunTime += process.burstTime;
}

void PrioWPreemp::storeProcesses(PCB *process, int numProc) {
	for (int i = 0; i < numProc; i++) {
		readyQueue.push_back(process[i]);
		totalRunTime += process[i].burstTime;
	}
}

void PrioWPreemp::schedule() {
	//sort processes according to their arrival time
	//this assumes that processes can have the same arrival time
	for (int i = 0; i < readyQueue.size(); i++) {
		int pos = i;
		for (int j = i + 1; j < readyQueue.size(); j++) {
			if (readyQueue[j].arrivalTime < readyQueue[pos].arrivalTime)
				pos = j;
		}
		PCB tmp = readyQueue[i];
		readyQueue[i] = readyQueue[pos];
		readyQueue[pos] = tmp;
	}
	runProc();
}

void PrioWPreemp::runProc() {
	bool finished = false;
	int arrTime = 0;		//keeps track of the minimum index for readyQueue
	int prevIndex;
	vector<PCB> vec;
	while (!finished) {
		//check if processes in readyQueue has been placed in vec
		if (arrTime != readyQueue.size()) {
			//stores all processes with the same arrival time to another list
			for (int i = arrTime; i < readyQueue.size(); i++) {
				if (readyQueue[i].arrivalTime == status.time) 
					vec.push_back(readyQueue[i]);
				else {
					arrTime = i;
					break;
				}
				if (i + 1 == readyQueue.size()) 
					arrTime = readyQueue.size();
			}
		}

		//picks highest priority within vec list
		int pos = 0;
		for (int i = 0; i < vec.size(); i++) {
			if (vec[pos].priority > vec[i].priority) {
				pos = i;
			}
		}

		//if status.curProc is pointing to something
		if (status.curProc.processNum != -1) {
			//see if switching to another process
			if (status.curProc.processNum != vec[pos].processNum) {
				//check if previous process is still present from vec
				bool stillThere = false;
				for (int i = 0; i < vec.size(); i++) {
					if (status.curProc.processNum == vec[i].processNum)
						stillThere = true;
				}
				//if still in vec do the following
				if (stillThere) {
					if (vec[prevIndex].wasPreempted == true) {
						vec[prevIndex].waitingTime = vec[prevIndex].waitingTime + (status.time - vec[prevIndex].lastExecTime - 1);
						vec[prevIndex].lastExecTime = status.time;
					}
					else {
						vec[prevIndex].lastExecTime = status.time;
						vec[prevIndex].wasPreempted = true;
					}
				}
				if (vec[pos].arrivalTime != status.time) {
					vec[pos].waitingTime = vec[pos].waitingTime + status.time - vec[pos].arrivalTime;
				}
				execProc.push_back(status.curProc); //status.curProc
				times.push_back(status.time - 1);
			}
			status.curProc = vec[pos];

			vec[pos].execTime++;
			status.time++;
			//if done executing the process
			if (vec[pos].execTime == vec[pos].burstTime) {
				//look for process in vec to erase
				for (int i = 0; i < vec.size(); i++) {
					if (vec[pos].processNum == vec[i].processNum) {
						vec.erase(vec.begin() + i);
						break;
					}
				}
			}
			//if runs out of totalRunTime, then exit
			if (status.time == totalRunTime + 1) {
				execProc.push_back(status.curProc);
				times.push_back(status.time - 1);
				finished = true;
			}
		}
		else {
			prevIndex = pos;
			status.curProc = vec[pos];
			vec[pos].execTime++;
			status.time++;
		}
	}
}

void PrioWPreemp::displayGantt() {
	cout << "Process Gantt Chart:" << endl;
	cout << "| ";
	int i;
	//display processes
	for (i = 0; i < execProc.size(); i++) {
		cout << execProc[i].processNum << " | ";
	}
	cout << endl;
	//display times
	for (i = 0; i < times.size(); i++) {
		if (times[i] > 9) {
			cout << times[i] << "  ";
		}
		else {
			cout << times[i] << "   ";
		}
	}
	getAveWait();
	cout << endl;
}

void PrioWPreemp::getAveWait() {
	for (int i = 0; i < execProc.size(); i++) 
		totalWaitTime += execProc[i].waitingTime;

	double aveWait = static_cast<float>(totalWaitTime) / readyQueue.size();
	cout << "\nAverage waiting time is: " << aveWait << endl;
}
