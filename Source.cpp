#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include <iomanip>
#include <map>
#include <numeric>
#include <typeinfo>
using namespace std;

int strtoi(string str) {
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
	vector<string> fileContent = rdFile(argv[2]);
	if (fileContent.empty() == true) {  // Check if there's anything in the file
		printf("The input file is either empty or cannot be opened\n");
		return 1;
	}
	else if ((fileContent.size() % 3) != 0) {  // Check if every process in the file has the right amount of info
		printf("The input file is incomplete\n");
		return 1;
	}

	vector<int> integerForm;
	for (int i = 0; i < fileContent.size(); ++i) {
		integerForm.push_back(strtoi(fileContent[i]));
	}
	map<int, int> processes;  // Mapping for more organization, process is key, arrival time is value
	vector<int> CPUburst;  // There were issues with stuffing a vector into a map, so the burst times are separated
	/*for (int i = 0; i < fileContent.size(); i + 3) {
		processes.insert(pair<int, int>(fileContent[i], fileContent[i + 1]));
		CPUburst.push_back(fileContent[i+2]);
	}*/

	string method = argv[3];
	if (method == "FCFS") {  // First come, first serve
		//FCFS(processes, CPUburst);
		cout << "FCFS" << endl;
	}
	else if (method == "SRTF") {  // Shortest remaining task first - preemptive
		cout << "SRTF" << endl;
	}
	else if (method == "RR") {  // Round robin
		cout << "RR" << endl;
	}
	else {
		printf("No CPU scheduling method selected\n");
		return 1;
	}
	return 0;
}