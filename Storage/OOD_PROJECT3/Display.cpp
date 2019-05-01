///////////////////////////////////////////////////////////////////////////////////////
// Display.cpp - class used to display converted html files in the browser           //                  
// ver 1.0                                                                           //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018                      //
///////////////////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include<iostream>
#include<conio.h>
#include "../Process/Process.h"

//default constructor
Display::Display()
{
}

//default destructor
Display::~Display()
{
}


//Function which takes html files as an argument and uses process class to create new process for each file
//and opens the html file in mozilla firefox
void Display::createProcess(std::vector<std::string> htmlFiles)
{
	Process p;
	p.title("test application");
	std::string appPath = "C:/Program Files/Mozilla Firefox/firefox.exe";  // path to application to start
	p.application(appPath);
	std::string cmdLine;
	std::cout << "\n Press any key to continue\n";
	_getche();

	for (size_t i = 0; i < htmlFiles.size(); ++i)
	{
		std::cout << "\n  starting process: \"" << appPath << "\"";
		cmdLine = "--new-window " + htmlFiles[i];
		p.commandLine(cmdLine);
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";

		p.create();
		CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
		p.setCallBackProcessing(callback);
		p.registerCallback();

		WaitForSingleObject(p.getProcessHandle(), INFINITE);  // wait for created process to terminate
	}

	std::cout << "\n  after OnExit";
	std::cout.flush();
}

//Test stub for display package
#ifdef TEST_DISPLAY

int main() {
	Display disp;
	std::vector<std::string> htmlFiles;
	htmlFiles.push_back("../convertedPages/test1.html");
	htmlFiles.push_back("../convertedPages/test2.html");
	htmlFiles.push_back("../convertedPages/test3.html");
	disp.createProcess(htmlFiles);
	_getche();
	return 0;
}
#endif // TEST_DISPLAY

