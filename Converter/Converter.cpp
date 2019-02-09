///////////////////////////////////////////////////////////////////////////
// Converter.cpp - class used convert a cpp file to html file           //                  
// ver 1.0                                                              //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018         //
//////////////////////////////////////////////////////////////////////////

#include "Converter.h"
#include <iostream>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include<regex>
#include<conio.h>
#include "../StringUtilities/StringUtilities.h"

//Using namespace to avoid ambiguity with converter in StringUtilities.h
namespace html {
	using namespace FileSystem;

	//Default Constructor
	Converter::Converter()
	{
	}

	//Default Destructor
	Converter::~Converter()
	{
	}

	//A function which writes cpp code to html file and if it succeeds return true, otherwise false
	bool Converter::writeHTML(string htmlFile, vector<string> fileContent) {
		ofstream webpageOutput(htmlFile);
		if (!webpageOutput.good()) {
			cout << "\n\n Unable to open the file";
			return false;
		}
		webpageOutput << "<!Doctype html >\n";
		webpageOutput << "<html>\n";
		webpageOutput << "<head>\n";
		webpageOutput << "</head>\n";
		webpageOutput << "<body>\n";
		webpageOutput << "<pre>\n";
		for (size_t i = 0; i < fileContent.size(); i++)
			webpageOutput << fileContent[i] << "\n";
		webpageOutput << "</pre>\n";
		webpageOutput << "</body>\n";
		webpageOutput << "</html>\n";

		webpageOutput.close();
		return true;
	}
	
	//Function to convert cpp files into html files
	vector<string> Converter::cpptohtml(vector<string> files)
	{
		vector<string> htmlFiles;
		vector<string> fileContent;
		
		ifstream in;
		
		for (size_t i = 0; i < files.size(); i++)
		{
			try {
				in.open(files[i]);
				if (!in.good()) {
					cout << "\n\n Unable to Open the file";
					return vector<string>();
				}
	
				for (string line; getline(in, line); )
				{
					while(line.find("<") != -1)
						line.replace(line.find("<"), 1, "&lt;");
					while (line.find(">") != -1)
						line.replace(line.find(">"), 1, "&gt;");
					fileContent.push_back(line);
				}

				std::string name = Path::getName(files[i]);
				std::string htmlFile = "../convertedPages/" + name + ".html";
				std::string Path = Path::getFullFileSpec(htmlFile);
				htmlFiles.push_back(Path);
				
				if (!writeHTML(htmlFile, fileContent))
					return vector <string>();
				
				in.close();
			}
			catch (std::exception& e) {
				cout << "\n Error opening/reading/writing to the file. \n Exception:-  "<<e.what();
			}
		}
		return htmlFiles;
	}

	//Function to display list of files
	void Converter::displayFiles(vector<string> files) {
		for (auto file : files)
			cout << "\n" << file;
		cout << "\n";
	}

	//Function to check if the directory exists, and if it does 
	//return all the files in the directory and sub-directory matching the pattern
	vector<string> Converter::initialChecks(ProcessCmdLine &pcl) {
		DirExplorerN de(pcl.path());
		for (auto patt : pcl.patterns())
		{
			de.addPattern(patt);
		}

		if (pcl.hasOption('s'))
		{
			de.recurse();
		}

		de.search();

		vector<string> Files = de.fileToProcess();
		vector<string> CppToHtmlFiles;

		cout << "\nList of files before applying regex";
		displayFiles(Files);

		for (auto regeX : pcl.regexes()) {
			regex s(regeX);
			for (auto file : Files) {
				auto fileName = Path::getName(file);
				if (regex_match(fileName,s))
					CppToHtmlFiles.push_back(file);
			}
			cout << "\nList of files after applying regex"<<regeX;
			//pcl.showRegexes();
			displayFiles(CppToHtmlFiles);
			/*cout << "\n Files to be processed:-" << CppToHtmlFiles.size();
			cout << "\n Following file will be converted into HTML file";
			for (auto file : CppToHtmlFiles)
				cout << "\n " << file;*/
		}

		

		return CppToHtmlFiles;
	}
}

//Test Stub for converter class
#ifdef TEST_CONVERTER1

using namespace Utilities;

//function in case user provide invalid command line
std::string customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]*";
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

int main(int argc, char** argv) {
	Utilities::ProcessCmdLine cmdargs(argc, argv);
	cmdargs.usage(customUsage());
	putline();

	Utilities::preface("Command Line: ");
	cmdargs.showCmdLine();

	cmdargs.showRegexes();

	if (cmdargs.parseError())
	{
		cmdargs.usage();
		std::cout << "\n\n";
		_getche();
		return 1;
	}
	
	std::vector<std::string> convertedFiles;
	html::Converter conv;
	convertedFiles = conv.cpptohtml(conv.initialChecks(cmdargs));

	std::cout << "\n  Converted files:\n";

	for (auto file : convertedFiles)
		std::cout << "  " << file << "\n";

	_getche();
	return 0;
}
#endif

