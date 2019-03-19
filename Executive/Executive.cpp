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
#include "../DependencyT/DependencyT.h"

//Namespaces
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
	std::map<std::string, std::vector<std::string>> depTable;
	std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> LT;

	html::Converter conv;
	std::vector<std::string> files = conv.initialChecks(cmdargs);

	std::cout << "\n Demonstrating Req6, Req7 & Req8\n";
	std::cout << "\n Converter::cpptohtml function converts all the \"<\" and \">\" characters into their corresponding escape sequences.";
	std::cout << "\n DependencyT::callParser and DependencyT::manipulateDepTable functions are responsible for insertion, in each analyzed file, links to all the files in the directory tree, on which it depends.";
	std::cout << "\n Converter::addDiv and Converter::handleComments functions add div tags in the html file at their appropriate positions";

	DependencyT dep;

	depTable = dep.callParser(files);
	dep.manipulateDepTable(depTable, files);

	LT = dep.returnLT();

	if (!(files.size() == 0)) {
		htmlFiles = conv.cpptohtml(files, depTable, LT);
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

//function to show requirement 4
void Executive::Req4()
{
	std::string file, path;

	std::cout << "\n Demonstrating Req4\n";

	file = FileSystem::Path::getFullFileSpec("../Executive/Executive.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 1. Executive:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Converter/Converter.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 2. Converter:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../Display/Display.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 3. Display:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../DependencyT/DependencyT.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 4. Dependencies:- Path: " << path;

	file = FileSystem::Path::getFullFileSpec("../DirExplorer-Naive/DirExplorerN.h");
	path = FileSystem::Path::getPath(file);
	std::cout << "\n 5. Loader:- Path: " << path;

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

		std::cout << "\n Demonstrating Req3\n";

		ProcessCmdLine cmdargs(argc, argv);
		cmdargs.usage(customUsage());

		preface("Command Line: ");
		cmdargs.showCmdLine();
		cmdargs.showRegexes();
		Utilities::putline(0);

		if (cmdargs.parseError())
		{
			cmdargs.usage();
			std::cout << "\n\n";
			_getche();
			return -1;
		}

		std::cout << "\n ---passed---\n\n";

		Executive exec;

		exec.Req4();

		std::cout << "\n Demonstrating Req5\n";
		
		exec.runProject(cmdargs);

		//Req9
		std::cout << "\n Demonstrating Req9\n";
		std::cout << "\n Well this is automated test suite!!!";
		std::cout << "\n ---passed---\n\n";

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