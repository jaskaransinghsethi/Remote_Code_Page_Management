//////////////////////////////////////////////////////////////////////////////////////
// Publisher.h  : Provides a publisher class to bind all the components together	//
// ver 1.0																			//
//																					//
// Application    : CSE-687 Project 3												//
// Platform       : Visual Studio 2017 Community Edition							//
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG			//
// Author         : Jaskaran Singh, EECS Department, Syracuse University			//
//                  315-925-9941, jsingh09@syr.edu									//
//////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
* ------------------ -
* Publisher package is created to execute all the functionalities implemented
* in the project.
*
* Public Interface:
* =================
* Publisher exe;											//To create an object of publisher class
* exe.startProject(argc, argv);								//To execute startProject() function to show 
*   														  all the functionalities of the project
* exe.getConvertedFiles(cmdargs);							//To get a list of converted files
*
* Required Files :
* ---------------
* Ipublisher.h,								//Ipublisher interface
* Publisher.cpp								//Publisher class	
* Converter.h, Converter.cpp				//Converter class
* Display.h, Display.cpp					//Display class
* CodeUtilities.h, CodeUtilities.cpp		//CodeUtilities class
* StringUtilities.h, StringUlities.cpp		//StringUtilities class
* DirExplorerN.h, DirExplorerN.cpp			//DirExplorer-Naive Class
*
*Maintenance History :
*--------------------
* ver 1.0 : 30th March 2019
* -first release
*/

#pragma once

#include "IPublisher.h"
#include <string>
#include <conio.h>
#include "../Converter/Converter.h"
#include"../Display/Display.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../StringUtilities/StringUtilities.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../DependencyT/DependencyT.h"
#include <vector>

using namespace Utilities;

//Publisher class implementing Ipublisher interface
class publisher : public Ipublisher{
public:
	publisher();
	~publisher() override;
	std::vector<std::string> startProject(int argc, char** argv) override;
	std::vector<std::string> getConvertedFiles(ProcessCmdLine& cmdargs);
	bool processCmdLineArg(int argc, char** argv);
};

//Default Constructor
inline publisher::publisher() {

}

//Default Destructor
inline publisher::~publisher() {

}

//A function which takes commandline argument and convert the given set of files into valid HTML files
inline std::vector<std::string> publisher::startProject(int argc, char** argv)
{
	if (processCmdLineArg(argc, argv)) {
		ProcessCmdLine cmdargs(argc, argv);
		return getConvertedFiles(cmdargs);
	}

	std::cout << "\n Invalid Command Line Arguments:";
	return std::vector<std::string>();
}

//A function to bind all the components together, perform initial checks, dependency analysis, convert files
//and return set of converted files
inline std::vector<std::string> publisher::getConvertedFiles(ProcessCmdLine & cmdargs)
{
	std::vector<std::string> htmlFiles;
	std::map<std::string, std::vector<std::string>> depTable;
	std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> LT;

	html::Converter conv;
	std::vector<std::string> files = conv.initialChecks(cmdargs);

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
		return std::vector<std::string>();
	}

	return htmlFiles;
}

//A function in case user provide invalid command line
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

//A function which parse command line arguments, return false if invalid arguments
inline bool publisher::processCmdLineArg(int argc, char** argv) {
	ProcessCmdLine cmdargs(argc, argv);
	cmdargs.usage(customUsage());

	preface("Command Line: ");
	cmdargs.showCmdLine();
	preface("Path: ");
	cmdargs.showPath();
	preface("Options: ");
	cmdargs.showOptions();
	preface("Patterns: ");
	cmdargs.showPatterns();
	preface("Regex: ");
	cmdargs.showRegexes();
	Utilities::putline();

	if (cmdargs.parseError())
	{
		cmdargs.usage();
		std::cout << "\n\n";
		_getche();
		return false;
	}

	return true;
}

// -----< Factory for returning Publisher as IPublisher* >--------------
Ipublisher* ObjectFactory::createClient() {
	return new publisher;
}