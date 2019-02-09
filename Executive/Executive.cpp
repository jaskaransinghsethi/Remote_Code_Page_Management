///////////////////////////////////////////////////////////////////////
// Executive.cpp - class used to implement project requirements      //                  
// ver 1.0                                                           //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018      //
///////////////////////////////////////////////////////////////////////

#include "Executive.h"
#include <iostream>
#include <vector>
#include<conio.h>
#include "../Converter/Converter.h"
#include"../Display/Display.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../StringUtilities/StringUtilities.h"
#include "../DirExplorer-Naive/DirExplorerN.h"

using namespace Utilities;

//Default Constructor
Executive::Executive()
{
}

//Default Destructor
Executive::~Executive()
{
}

//function to show all the rerequirements for OOD project 1
void Executive::runProject(ProcessCmdLine& cmdargs) {
	
	std::vector<std::string> htmlFiles;

	html::Converter conv;
	std::vector<std::string> files = conv.initialChecks(cmdargs);

	if (!(files.size() == 0)) {
		htmlFiles = conv.cpptohtml(files);
	}
	else {
		std::cout << "\n No files found";
		std::cout << "\n ---passed---\n\n";
		return;
	}

	std::cout << "\n";

	Display d;
	d.createProcess(htmlFiles);
	std::cout << "\n ---passed---\n\n";

}

//function to show requirement 3
void Executive::Req3()
{
	std::string file, path;

	std::cout << "\n Demonstrating Req3";

	file = FileSystem::Path::getFullFileSpec("../Executive/Executive.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 1. Executive:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Converter/Converter.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 2. Converter:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Display/Display.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 3. Display:- Path: " << path;
	std::cout << "\n ---passed---\n\n";
}

//function in case user provide invalid command line
std::string customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]* [/Regex]";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s - walk directory recursively";
	//usage += "\n      /h - hide empty directories";
	//usage += "\n      /a - on stopping, show all files in current directory";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n";
	return usage;
}

//Test Stub for Executive Package
#ifdef TEST_EXECUTIVE

//main function to implement all the requirements for OOD project 1
int main(int argc, char** argv) {
	try {

		std::cout << "\n Demonstrating Req4\n\n";

		ProcessCmdLine cmdargs(argc, argv);
		cmdargs.usage(customUsage());

		preface("Command Line: ");
		cmdargs.showCmdLine();
		cmdargs.showRegexes();
		putline();

		if (cmdargs.parseError())
		{
			cmdargs.usage();
			std::cout << "\n\n";
			_getche();
			return 1;
		}
		std::cout << "\n ---passed---\n\n";
		
		std::cout << "\n Demonstrating Req5, Req6 and Req7\n\n";
		Executive exec;
		exec.runProject(cmdargs);
		
		_getche();
		return 0;
	}
	catch (std::exception ex) {
		std::cout << "\nStandard exception caught" << ex.what();
		return -1;
	}
	catch (...) {
		std::cout << "\n Unknown exception caught";
		return -1;
	}
}
#endif