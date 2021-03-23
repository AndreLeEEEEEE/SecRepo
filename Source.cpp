#include <iostream>  // General printing library
#include <fstream>  // Reading in files
#include <sstream>  // Converting string to int
#include <string>  // Use of strings
#include <queue>  // Use of queues
#include <vector>  // Use of vectors
#include <map>  // Use of maps
#include <numeric>  // Use of summation
#include <iomanip>  // Use of tables
using namespace std;  // Eliminates need to type std

int strtoi(string str) {
	// I have to make my own string to int function since 'stoi' doesn't work in a linux environment
	stringstream scasti(str);
	int temp;
	scasti >> temp;
	return temp;
}

vector<string> rdFile(string fileName) {
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
	for (int s = 0; s < amount; ++s) {
		printf("*");
		if (gantt) {
			printf(" ");
		}
	}
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
	

}

void FCFS(vector<int> process, vector<int> burst) {
	queue<int> ready;
	int CPU_sum = accumulate(burst.begin(), burst.end(), 0);  // Sum of all burst times
	vector<int> finish(process.size(), 0);
	vector<int> times;
	vector<int> store_b = burst;
	int CPU = 0;
	int j = 0;  // j is the current ms
	bool allDone = false;  // While loop condition

	while (!allDone) {  // Go until all processes have finished
		// Check if any process arrives at this time and place at back of queue
		for (int k = 0; k < process.size(); ++k) {
			// Check arrival times
			if (process[k] == j) {
				// Push process onto queue if arrived
				ready.push(k+1);
				break;
			}
		}

		if (ready.empty()) {  // If no process is in the ready queue, time marches
			times.push_back(0);
		}
		else if (burst[ready.front() - 1] > 0) {
			// Denote when a new process starts
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
				finish[ready.front() - 1] = j + 1;
				ready.pop();

			}
		}

		++j;
		allDone = true;
		for (int k = 0; k < finish.size(); ++k) {
			// Check if any process, arrived or not, hasn't finished
			if (finish[k] == 0) {
				allDone = false;
				break;
			}
		}
	}

	vector<int> context(finish.size(), 0);
	Gantt(process, store_b, finish, times, context, "FCFS");
}

int main() {
	//int argc, char* argv[]
	//vector<string> fileContent = rdFile(argv[2]);
	vector<string> fileContent = rdFile("Input.txt");

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

	FCFS(processes, CPUburst);
	// Due to being a command line variable, argv[3] is technically is string, but typeid says Pc
	// To officially make it a string, I have to store it in string variable
	//string method = argv[3];
	//if (method == "FCFS") {  // First come, first serve
	//	//FCFS(processes, CPUburst);
	//	cout << "FCFS" << endl;
	//}
	//else if (method == "SRTF") {  // Shortest remaining task first - preemptive
	//	cout << "SRTF" << endl;
	//}
	//else if (method == "RR") {  // Round robin
	//	cout << "RR" << endl;
	//}
	//else {
	//	printf("No CPU scheduling method selected\n");
	//	return 1;
	//}
	return 0;
}