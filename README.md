Name: Andre Le
Student ID: 109069441
Class: CSCI 3453 001
Due Date: Mar 28, 2021

************************************************
Description
************************************************
This program simulates three methods of CPU 
scheduling: FCFS, SRTF, and RR. The program uses 
command line arguments to receive an input file
and decide which scheduling method to use. The end
result of the program is a Gantt chart of the 
processes and other relevant information using
the selected scheduling method.

************************************************
Program Structure
************************************************
Name: Source.cpp
    The program uses command line arguments to
    receive its input. The two most important
    inputs are a file name and string. The program
    reads in the file to obtain all the processes
    and their arrival time and CPU burst time. The
    string argument decides whether the program
    will schedule the processes with. Once done,
    the program will print out a Gantt chart of the
    results alongside a table and other lines of 
    calculated data such as wait times and 
    turnaround times.

************************************************
Platforms
************************************************
This program was made on Visual Studio and
compiled and tested on csegrid.

************************************************
Status
************************************************
This program works perfectly on csegrid.ucdenver.pvt.

************************************************
How to compile
************************************************
Use the makefile or g++ to compile main.cpp
