// Andre Le, March 27, 2021, Student ID: 109069441, CSCI 3453
#include <iostream>  // General printing library
#include <fstream>  // Reading in files
#include <sstream>  // Converting string to int
#include <string>  // Use of strings
#include <queue>  // Use of queues
#include <vector>  // Use of vectors
#include <map>  // Use of maps
#include <numeric>  // Use of summation
#include <iomanip>  // Use of tables
#include <limits>  // Use of integer infinity
#include <algorithm>  // Use of count function in a unix environment
using namespace std;  // Eliminates need to type std

int strtoi(string str) {
	// I have to make my own string to int function since 'stoi' doesn't work in a linux environment
	stringstream scasti(str);
	int temp;
	scasti >> temp;
	return temp;
}

vector<string> rdFile(string fileName) {
	// Read in the file
	ifstream theFile(fileName);
	vector<string> contents;
	string word;
	while (theFile >> word) {  // While there's something to be read
		contents.push_back(word);
	}
	theFile.close();

	return contents;
}

void printStar(int amount, bool gantt) {
	// Print multiple stars for the title or the gantt chart
	for (int s = 0; s < amount; ++s) {
		printf("*");
		if (gantt) {
			printf(" ");
		}
	}
}

float averageVector(vector<int> vec) {
	// Calculate the floating point sum of a vector
	float f_sum = float(accumulate(vec.begin(), vec.end(), 0));
	return f_sum / vec.size();
}

void Gantt(vector<int> a, vector<int> b, vector<int> f, vector<int> t, vector<int> c, string algo) {
	printStar(60, false);
	printf("\n");
	printStar(16, false);
	printf(" Scheduling algorithm : %s ", algo.c_str());
	printStar(15, false);
	printf("\n");
	printStar(60, false);
	printf("\n\n");
	// Print out gantt
	for (int i = 0; i < t.size(); ++i) {
		// Print the processes in their proper places first
		if (t[i] != 0) {
			printf("P%d", t[i]);
		}
		else {
			printf("  ");
		}
	}
	printf("\n");
	printStar(t.size(), true);  // Print out the elasped ms
	printf("\n\n");
	// Print out table
	cout << setw(5) << "pid" << setw(9) << "arrival" << setw(11) << "CPU-burst" << setw(8) << "finish";
	cout << setw(14) << "waiting time" << setw(13) << "turn around" << setw(17) << "No. of Context\n";
	vector<int> wait;
	vector<int> turn;
	for (int i = 0; i < a.size(); ++i) {
		turn.push_back(f[i] - a[i]);
		wait.push_back(turn[i] - b[i]);
		cout << setw(5) << i+1 << setw(9) << a[i] << setw(11) << b[i] << setw(8) << f[i];
		cout << setw(14) << wait[i] << setw(13) << turn[i] << setw(17) << c[i] << endl;
	}
	printf("\n");
	// Additional stats
	printf("Average CPU burst time = %f ms\n", averageVector(b));
	printf("Average waiting time = %f ms\n", averageVector(wait));
	printf("Average turn around time = %f ms\n", averageVector(turn));
	printf("Total No. of Context Switching Performed = %d\n", accumulate(c.begin(), c.end(), 0));
}

void SRTF(vector<int> process, vector<int> burst) {
	vector<int> f_queue;  // Holds process ids 
	// A real queue doesn't quite work when I have to search through it to find the smallest burst
	vector<int> finish(process.size(), 0);
	vector<int> times;  // Keep track of when processes start
	vector<int> store_b = burst;  // We need to keep a unmodified copy
	vector<int> context(process.size(), 0);  // Keeps track of when process undergo a context switch
	int CPU = 0;  // Keep track of which process is at the front
	int j = 0;  // j is the current ms
	bool allDone = false;  // While loop condition

	while (!allDone) {  // Go until all processes have finished
		// Check if any process arrives at this time and place at back of queue
		for (int k = 0; k < process.size(); ++k) {
			// Check arrival times
			if (process[k] == j) {
				// Push process onto queue if arrived
				f_queue.push_back(k + 1);
			}
		}

		// Find the process with the shortest time remaining
		if (f_queue.empty()) {  // If no process is in the ready queue, time marches
			times.push_back(0);
		}
		else {
			int shortest = numeric_limits<int>::max();  // smallest burst
			int displace = 0;  // Tells the program how many processes to move back
			for (int i = 0; i < f_queue.size(); ++i) {
				if (burst[f_queue[i] - 1] < shortest) {
					shortest = burst[f_queue[i] - 1];  // Find shortest burst
					displace = i;
				}
			}
			// Move the process with the smallest burst to the front
			for (int i = 0; i < displace; ++i) {
				f_queue.push_back(f_queue.front());
				f_queue.erase(f_queue.begin());
			}
			// Denote if new process starts
			if (CPU != f_queue.front()) {
				CPU = f_queue.front();
				times.push_back(CPU);
			}
			else {
				times.push_back(0);
			}
			// If current process still has burst left
			--burst[f_queue.front() - 1];
			if (burst[f_queue.front() - 1] == 0) {
				// If process is finished
				finish[f_queue.front() - 1] = j + 1;  // Store process' finish time
				f_queue.erase(f_queue.begin());
			}
		}

		++j;
		allDone = true;
		for (int k = 0; k < finish.size(); ++k) {
			// Check if any process, arrived or not, hasn't finished
			if (finish[k] == 0) {
				// A single unfinished process means keep going
				allDone = false;
				break;
			}
		}
	}

	for (int i = 0; i < process.size(); ++i) {
		// Update contexts based on appearances in times vector
		if (count(times.begin(), times.end(), i + 1) > 1) {
			context[i] = count(times.begin(), times.end(), i + 1) - 1;
		}
	}

	Gantt(process, store_b, finish, times, context, "SRTF");  // Print out in gantt chart form
}

void FCFS(vector<int> process, vector<int> burst) {
	queue<int> ready;  // Holds process ids
	vector<int> finish(process.size(), 0);
	vector<int> times;  // Keep track of when processes start
	vector<int> store_b = burst;  // We need to keep a unmodified copy
	int CPU = 0;  // Keep track of which process is at the front
	int j = 0;  // j is the current ms
	bool allDone = false;  // While loop condition

	while (!allDone) {  // Go until all processes have finished
		// Check if any process arrives at this time and place at back of queue
		for (int k = 0; k < process.size(); ++k) {
			// Check arrival times
			if (process[k] == j) {
				// Push process onto queue if arrived
				ready.push(k+1);
			}
		}

		if (ready.empty()) {  // If no process is in the ready queue, time marches
			times.push_back(0);
		}
		else if (burst[ready.front() - 1] > 0) {
			// Denote if new process starts
			if (CPU != ready.front()) {
				CPU = ready.front();
				times.push_back(CPU);
			}
			else {
				times.push_back(0);
			}
			// If current process still has burst left
			--burst[ready.front() - 1];
			if (burst[ready.front() - 1] == 0) {
				// If process is finished
				finish[ready.front() - 1] = j + 1;  // Store process' finish time
				ready.pop();
			}
		}

		++j;
		allDone = true;
		for (int k = 0; k < finish.size(); ++k) {
			// Check if any process, arrived or not, hasn't finished
			if (finish[k] == 0) {
				// A single unfinished process means keep going
				allDone = false;
				break;
			}
		}
	}

	vector<int> context(finish.size(), 0);  // FSFC has no context switches
	Gantt(process, store_b, finish, times, context, "FCFS");  // Print out in gantt chart form
}

void RR(vector<int> process, vector<int> burst, int quantum) {
	queue<int> ready;  // Holds process ids
	vector<int> finish(process.size(), 0);
	vector<int> times;  // Keep track of when processes start
	vector<int> store_b = burst;  // We need to keep a unmodified copy
	vector<int> context(process.size(), 0);  // Keeps track of when process undergo a context switch
	int CPU = 0, j = 0, q = 0;  // Keep track of which process is at the front; // j is the current ms; // q is the building quantum time
	bool allDone = false;  // While loop condition

	while (!allDone) {  // Go until all processes have finished
		// Check if any process arrives at this time and place at back of queue
		for (int k = 0; k < process.size(); ++k) {
			// Check arrival times
			if (process[k] == j) {
				// Push process onto queue if arrived
				ready.push(k + 1);
			}
		}

		if (ready.empty()) {  // If no process is in the ready queue, time marches
			times.push_back(0);
		}
		else {
			// Denote if new process starts
			if (CPU != ready.front()) {
				CPU = ready.front();
				times.push_back(CPU);
			}
			else {
				times.push_back(0);
			}
			// If current process still has burst left and/or quantum left
			--burst[ready.front() - 1];
			++q;
			if (burst[ready.front() - 1] == 0) {
				// If process is finished
				finish[ready.front() - 1] = j + 1;  // Store process' finish time
				ready.pop();
				q = 0;
			}
			else if (q == quantum) {
				// Process isn't done
				++context[ready.front() - 1];  // Increment No. of contexts
				ready.push(ready.front());  // Put process on back
				ready.pop();  // Before removing from front
				q = 0;
			}
		}

		++j;
		allDone = true;
		for (int k = 0; k < finish.size(); ++k) {
			// Check if any process, arrived or not, hasn't finished
			if (finish[k] == 0) {
				// A single unfinished process means keep going
				allDone = false;
				break;
			}
		}
	}

	Gantt(process, store_b, finish, times, context, "RR");  // Print out in gantt chart form
}

int main(int argc, char* argv[]) {
	vector<string> fileContent = rdFile(argv[2]);

	if (fileContent.empty() == true) {  // Check if there's anything in the file
		printf("The input file is either empty or cannot be opened\n");
		return 1;
	}
	else if ((fileContent.size() % 3) != 0) {  // Check if every process in the file has the right amount of info
		printf("The input file is incomplete\n");
		return 1;
	}

	vector<int> intContent;
	/* I have to convert fileContent from string to int in main because for some reason
	* doing so in rdFile (either with function calls or raw statements) will yield a
	* 'bad_alloc' error.*/
	for (int i = 0; i < fileContent.size(); ++i) {
		intContent.push_back(strtoi(fileContent[i]));
	}
	fileContent.clear();

	vector<int> processes;  // Mapping for more organization, process is key, arrival time is value
	vector<int> CPUburst;  // There were issues with stuffing a vector into a map, so the burst times are separated
	for (int i = 1; i < intContent.size(); i = i + 3) {
		processes.push_back(intContent[i]);
		CPUburst.push_back(intContent[i + 1]);
	}

	// Due to being a command line variable, argv[3] is technically is string, but typeid says Pc
	// To officially make it a string, I have to store it in string variable
	string method = argv[3];
	if (method == "FCFS") {  // First come, first serve
		FCFS(processes, CPUburst);
	}
	else if (method == "SRTF") {  // Shortest remaining task first - preemptive
		SRTF(processes, CPUburst);
	}
	else if (method == "RR") {  // Round robin
		string t_q = argv[4];
		RR(processes, CPUburst, strtoi(t_q));
	}
	else {
		printf("No CPU scheduling method selected\n");
		return 1;
	}
	return 0;
}
