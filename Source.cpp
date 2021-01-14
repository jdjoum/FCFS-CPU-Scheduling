// FCFS Scheduling Algorithm Djoum
#include <list>
#include <string>
#include <iostream>
using namespace std;

struct schedProcess
{
	string array;
	// Arrays for CPU and IO
	int CPUTime[10], IOTime[10];
	// CPU and IO indexes
	int CPUPos, IOPos;
	// Current CPU Burst Length
	int tempCPUBurst;
	// Current IO Burst Length
	int tempIOBurst;
	// Response Time
	int responseTime;
	// Waiting Time
	int waitTime;
	// Turnaround Time
	int turnTime;
};

// Defining the Processes
schedProcess p1 = { "P1",{ 5,3,5,4,6,4,3,4 },
{27,31,43,18,22,26,24},0,0,0,0,-1,0,0 },
p2 = { "P2",{ 4,5,7,12,9,4,9,7,8},
{ 48,44,42,37,76,41,31,43 }, 0,0,0,0,-1,0,0 },
p3 = { "P3", { 8,12,8,14,4,5,14,5,6 },
{ 33,41,65,21,61,18,26,31 },0,0,0,0,-1,0,0 },
p4 = { "P4", { 3,4,5,3,4,5,6,5,3 },
{ 35,41,45,51,61,54,82,77 },0,0,0,0,-1,0,0 },
p5 = { "P5",{ 16,17,5,16,7,13,11,6,3,4 },
{ 24,21,36,26,31,28,21,13,11 },0,0,0,0,-1,0,0 },
p6 = { "P6", { 11,4,5,6,7,9,12,15,8 },
{ 22,8,10,12,14,18,24,30 },0,0,0,0,-1,0,0 },
p7 = { "P7", { 14,17,11,15,4,7,16,10 },
{ 46,41,42,21,32,19,33 },0,0,0,0,-1,0,0 },
p8 = { "P8", { 4,5,6,14,16,6 },
{ 14,33,51,73,87 },0,0,0,0,-1,0,0 };

// Function Prototypes:
// Set up the Ready Queue Initially
void setupReadyQueue(list<schedProcess>& ready);
// Compare 2 Processes and sort
bool compareProcess(const schedProcess& p1, const schedProcess& p2);
// Add to the wait time
void increaseWait(list<schedProcess>& ready);
// Print the Ready Queue
void printReadyQueue(list<schedProcess>& ready);
// Prints IO Queue
void printIOQueue(list<schedProcess>& io);
// Print finished process
void printCompleteProcess(list<schedProcess>& complete);
// Get the Next CPU Burst of a process
int getNextCPUBurst(list<schedProcess>::iterator p, int pos);
// Get the Next IO Burst of a process
int getNextIOBurst(schedProcess p, int pos);

// Function to run FCFS Algorithm
int main()
{
	list<schedProcess> readyList;
	list<schedProcess> ioList;
	list<schedProcess> completedList;
	schedProcess tempCPUProcess;
	// Initializing control variables
	bool erased = true;
	bool idle = false;
	// Variable to sum (TR/TW/TTR)
	int totalAvgCalc = 0;
	// Holds how CPU Idle time
	int cpuIdle = 0;
	// Initializing the time variable
	int time = 0;
	// Counter for current CPU Burst index
	int j = 0;
	// Total number of process
	int numProcesses = 8;
	// Set up the Ready Queue
	setupReadyQueue(readyList);

	// Save the first CPU burst of each process
	for (list<schedProcess>::iterator i = readyList.begin(); i != readyList.end(); ++i)
	{
		i->tempCPUBurst = getNextCPUBurst(i, i->CPUPos);
	}

	// FCFS 
	while (!ioList.empty() || !readyList.empty())
	{
		// CPU Idle Case
		if (idle == true)
		{
			time++;
			cpuIdle++;
			if (!readyList.empty())
			{
				// Process waiting in Ready Queue
				idle = false;
				increaseWait(readyList);
			}
			if (!ioList.empty())
			{
				// Decrease the IO burst of processes in IO queue
				list<schedProcess>::iterator i = ioList.begin();
				while (i != ioList.end() || erased == true)
				{
					// If there's IO time left
					if (i->tempIOBurst > 1)
					{
						i->tempIOBurst--;
						erased = false;
						++i;
					}
					// IO Time Completed
					else
					{
						if (i->tempIOBurst == 1)
						{
							i->tempCPUBurst--;
						}
						i->CPUPos++;
						i->tempCPUBurst = getNextCPUBurst(i, i->CPUPos);
						// Process goes back to Ready Queue
						readyList.push_back(*i);
						idle = false;
						i = ioList.erase(i);
						if (i != ioList.end()) 
						{
							erased = true;
						}
					}
				}
			}
			
		}
		// CPU is Running
		else
		{
			// CPU Goes Idle Case
			if (readyList.empty())
			{
				idle = true;
				cout << "Current Time:" << time << endl << endl;
				cout << "Next Process in CPU: " << "-IDLE" << endl << endl;
				printReadyQueue(readyList);
				printIOQueue(ioList);
				printCompleteProcess(completedList);
			}
			// CPU Processing from the start of the Ready Queue
			else
			{
				idle = false;
				cout << "Current Time:" << time << endl << endl;
				tempCPUProcess = readyList.front();
				if (tempCPUProcess.responseTime < 0)
				{
					tempCPUProcess.responseTime = time;
				}
				j = tempCPUProcess.tempCPUBurst;
				readyList.pop_front();
				cout << "Next Process in CPU: " << tempCPUProcess.array << endl << endl;
				printReadyQueue(readyList);
				printIOQueue(ioList);
				printCompleteProcess(completedList);

				// Current CPU Burst Loop
				for (j; j > 0; j--)
				{
					tempCPUProcess.tempCPUBurst--;
					time++;
					increaseWait(readyList);
					if (!ioList.empty())
					{
						// Traverse all the processes in the IO Queue, then decrease their IO bursts
						list<schedProcess>::iterator i = ioList.begin();
						while (i != ioList.end() || erased == true)
						{
							// If there is currently IO time decrease the IO Burst
							if (i->tempIOBurst > 1)
							{
								i->tempIOBurst--;
								erased = false;
								++i;
							}
							// Process has fully executed IO Time or has single time unit in IO
							else
							{
								if (i->tempIOBurst == 1)
								{
									i->tempIOBurst--;
								}
								i->CPUPos++;
								i->tempCPUBurst = getNextCPUBurst(i, i->CPUPos);
								// Put process back in the Ready Queue
								readyList.push_back(*i);
								i = ioList.erase(i);
								if (i != ioList.end())
								{
									erased = true;
								}
							}
						}
					}
				}
				// Process has been completely executed CPU and IO, added to completed list if no IO left
				if (tempCPUProcess.tempCPUBurst == 0)
				{
					tempCPUProcess.tempIOBurst = getNextIOBurst(tempCPUProcess, tempCPUProcess.IOPos);
					tempCPUProcess.IOPos++;
					if (tempCPUProcess.tempIOBurst == 0)
					{
						if (completedList.empty())
						{
							tempCPUProcess.turnTime = time;
							completedList.push_front(tempCPUProcess);
						}
						else
						{
							tempCPUProcess.turnTime = time;
							completedList.push_back(tempCPUProcess);
							completedList.sort(compareProcess);
						}
					}
					else
					{
						// Process Returns to IO
						ioList.push_back(tempCPUProcess);
					}
				}
			}
		}
	}
	cout << "Current Time: " << time << endl << endl;
	cout << "Next Process in CPU: " << "-IDLE" << endl << endl;
	idle = true;
	printReadyQueue(readyList);
	printIOQueue(ioList);
	printCompleteProcess(completedList);

	// Display final values: TR,TW,AND TTR
	cout << "\nFINISHED\n\n";
	cout << "---FCFS RESULTS---" << endl << endl;
	cout << "-Total time: " << time << endl << endl;
	cout << "-CPU Utilization: " << 100 * (time - cpuIdle) / (float)time << endl << endl;
	cout << "-RESPONSE TIME: " << endl << endl;
	for (list<schedProcess>::iterator i = completedList.begin(); i != completedList.end(); ++i)
	{
		cout << " " << i->array << "\t";
	}
	cout << endl;
	for (list<schedProcess>::iterator i = completedList.begin(); i != completedList.end(); ++i)
	{
		cout << " " << i->responseTime << "\t";
		totalAvgCalc += i->responseTime;
	}
	cout << endl << endl;
	cout << "-Average Response Time: \t" << (float)totalAvgCalc / (float)numProcesses << endl << endl;
	totalAvgCalc = 0;
	cout << "-WAITING TIME: " << endl << endl;
	for (list<schedProcess>::iterator i = completedList.begin(); i != completedList.end(); ++i)
	{
		cout << " " << i->array << "\t";
	}
	cout << endl;
	for (list<schedProcess>::iterator i = completedList.begin(); i != completedList.end(); ++i)
	{
		cout << " " << i->waitTime << "\t";
		totalAvgCalc += i->waitTime;
	}
	cout << endl << endl;
	cout << "-Average Waiting Time: \t\t" << (float)totalAvgCalc / (float)numProcesses << endl << endl;
	totalAvgCalc = 0;
	cout << "-TURNAROUND TIME:" << endl << endl;
	for (list<schedProcess>::iterator i = completedList.begin(); i != completedList.end(); ++i)
	{
		cout << " " << i->array << "\t";
	}
	cout << endl;
	for (list<schedProcess>::iterator i = completedList.begin(); i != completedList.end(); ++i)
	{
		cout << " " << i->turnTime << "\t";
		totalAvgCalc += i->turnTime;
	}
	cout << endl << endl;
	cout << "-Average Turnaround time: \t" << (float)totalAvgCalc / (float)numProcesses << endl << endl;
	return 0;
}

void printReadyQueue(list<schedProcess>& ready)
{
	cout << "List of processes in the ready queue: " << endl << endl;
	cout << "\tProcess\t\tBurst" << endl << endl;
	if (ready.empty())
	{
		cout << "\t-Empty" << endl << endl;
	}
	else
	{
		for (list<schedProcess>::iterator i = ready.begin(); i != ready.end(); ++i)
		{
			cout << "\t" << i->array;
			cout << "\t\t" << i->tempCPUBurst;
			cout << endl;
		}
	}
	cout << "-------------------------------------------" << endl << endl;
}

void printIOQueue(list<schedProcess>& io)
{
	cout << "List of processes in the IO queue: " << endl << endl;
	cout << "\tProces\t\tRemaining IO Time" << endl << endl;
	if (io.empty())
	{
		cout << "\t-EMPTY" << endl << endl;
	}
	else
	{
		for (list<schedProcess>::iterator i = io.begin(); i != io.end(); ++i)
		{
			cout << "\t" << i->array;
			cout << "\t\t" << i->tempIOBurst;
			cout << endl << endl;
		}
	}
}

void printCompleteProcess(list<schedProcess>& complete)
{
	if (!complete.empty())
	{
		cout << "-COMPLETED:";
		for (list<schedProcess>::iterator i = complete.begin(); i != complete.end(); ++i)
		{
			cout << i->array << ", ";
		}
		cout << endl << endl;
	}
	else
	{
		cout << "No Completed Processes" << endl << endl;
	}
}

void setupReadyQueue(list<schedProcess>& ready)
{
	ready.push_front(p8);
	ready.push_front(p7);
	ready.push_front(p6);
	ready.push_front(p5);
	ready.push_front(p4);
	ready.push_front(p3);
	ready.push_front(p2);
	ready.push_front(p1);
}

bool compareProcess(const schedProcess& p1, const schedProcess& p2)
{
	if (p1.array != p2.array)
	{
		return p1.array < p2.array;
	}
}

void increaseWait(list<schedProcess>& ready)
{
	for (list<schedProcess>::iterator i = ready.begin(); i != ready.end(); ++i)
	{
		i->waitTime++;
	}
}

int getNextCPUBurst(list<schedProcess>::iterator p, int pos)
{
	p->tempCPUBurst = p->CPUTime[pos];
	pos++;
	return p->tempCPUBurst;
}

int getNextIOBurst(schedProcess p, int pos)
{
	p.tempIOBurst = p.IOTime[pos];
	pos++;
	return p.tempIOBurst;
}
