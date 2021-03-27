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
    The program first uses the uname function and
    sys/utsname.h to print out system information.
    Second, the program goes into the proc directory
    and reads in certain values from certain files.
    Then, it converts those values from strings to
    integers for printing.

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
