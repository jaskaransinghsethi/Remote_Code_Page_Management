#pragma once
///////////////////////////////////////////////////////////////////////////////////////
// Display.h - Display package  display converted html files in the browser        //                  
// ver 1.0                                                                           //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018                      //
///////////////////////////////////////////////////////////////////////////////////////
/*
*
*Package Operations:
* -------------------
* Display provides a class, of the same name, that has one function createProcess which
*takes vector of string of html files and uses process package to create a new process
*for each file and open it in google chrome
*
* Public Interface:
* =================
* Display disp;                    //To create an object of display class
* disp.createProcess(std::vector<std::string> htmlFiles); //To open htmlfiles in the web browser using Process class
*
*Required Files:
* ---------------
*Process.h, Process.cpp    //Process class
*
*Maintenance History:
* --------------------
* - Made createProcess function
* ver 1.0 : 26 Jan 2019
* - first release
*/
#include<vector>

class Display
{
public:
	Display();
	~Display();
	void createProcess(std::vector<std::string> htmlFiles);
};

