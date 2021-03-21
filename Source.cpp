#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <iomanip>
#include <map>
#include <numeric>
using namespace std;

vector<int> rdFile(string fileName) {
	ifstream theFile(fileName);
	vector<int> contents;
	string word;
	if (theFile.is_open()) {
		while (theFile >> word) {  // While there's something to be read
			stringstream scasti(word);
			int temp;
			scasti >> temp;
			contents.push_back(temp);
		}
		theFile.close();
	}
	return contents;
}

int turnAround(int finish, int arrival) {
	return finish - arrival;
}

void FCFS(map<int, int> process, vector<int> burst) {
	queue<int> ready;
	int CPU_sum = accumulate(burst.begin(), burst.end(), 0);  // Sum of all burst times
	vector<int> finish(process.size(), 0);
	int CPU;

	for (int j = 0; j < CPU_sum; ++j) {  // j is the current ms
		// Check if any process arrives at this time and place at back of queue
		for (int k = 1; k < process.size() + 1; ++k) {
			// Check arrival times
			if (process[k] == j) {
				// Push process onto queue if arrived
				ready.push(k);
				CPU = ready.front();
			}
		}

		if (ready.empty()) {
			continue;
		}
		else if (burst[CPU - 1] > 0) {
			// If current process still has burst left
			--burst[CPU - 1];
			if (burst[CPU - 1] == 0) {
				// If process is finished
				ready.pop();
				finish[CPU - 1] = j;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	for (int i = 0; i < argc; ++i) {
		cout << argv[i] << endl;
	}
	vector<int> fileContent = rdFile(argv[2]);
	if (fileContent.empty() == true) {  // Check if there's anything in the file
		printf("The input file is either empty or cannot be opened\n");
		return 1;
	}
	else if ((fileContent.size() % 3) != 0) {  // Check if every process in the file has the right amount of info
		printf("The input file is incomplete\n");
		return 1;
	}
	map<int, int> processes;  // Mapping for more organization, process is key, arrival time is value
	vector<int> CPUburst;  // There were issues with stuffing a vector into a map, so the burst times are separated
	for (int i = 0; i < fileContent.size(); i + 3) {
		processes.insert(pair<int, int>(fileContent[i], fileContent[i + 1]));
		CPUburst.push_back(fileContent[i+2]);
	}

	// Remove later
	for (int i = 0; i < processes.size(); ++i) {
		cout << processes[i + 1] << endl;
	}
	for (int i = 0; i < CPUburst.size(); ++i) {
		cout << CPUburst[i] << endl;
	}

	if (argv[3] == "FCFS") {  // First come, first serve
		//FCFS(processes, CPUburst);
		cout << "FCFS" << endl;
	}
	else if (argv[3] == "SRTF") {  // Shortest remaining task first - preemptive
		cout << "SRTF" << endl;
	}
	else if (argv[3] == "RR") {  // Round robin
		cout << "RR" << endl;
	}
	else {
		printf("No CPU scheduling method selected\n");
		return 1;
	}
	return 0;
}