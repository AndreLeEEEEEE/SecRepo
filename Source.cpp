#include <iostream>  // General printing library
#include <fstream>  // Reading in files
#include <sstream>  // Converting string to int
#include <string>  // Use of strings
#include <queue>  // Use of queues
#include <vector>  // Use of vectors
#include <iomanip>  // Allows command line arguments
#include <map>  // Use of maps
#include <numeric>  // Use of summation
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

	vector<int> intContent;
	/* I have to convert fileContent from string to int in main because for some reason
	* doing so in rdFile (either with function calls or raw statements) will yield a
	* 'bad_alloc' error.*/
	for (int i = 0; i < fileContent.size(); ++i) {
		intContent.push_back(strtoi(fileContent[i]));
	}

	map<int, int> processes;  // Mapping for more organization, process is key, arrival time is value
	vector<int> CPUburst;  // There were issues with stuffing a vector into a map, so the burst times are separated
	/*for (int i = 0; i < intContent.size(); i + 3) {
		processes.insert(pair<int, int>(intContent[i], intContent[i + 1]));
		CPUburst.push_back(intContent[i+2]);
	}*/

	// Due to being a command line variable, argv[3] is technically is string, but typeid says Pc
	// To officially make it a string, I have to store it in string variable
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