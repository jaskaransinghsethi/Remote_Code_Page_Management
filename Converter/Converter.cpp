///////////////////////////////////////////////////////////////////////////
// Converter.cpp - class used convert a cpp file to html file           //   
// ver 1.3                                                              //
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

	//Function to handle classes in handleSwitch function
	void Converter::handleClass(size_t &i, vector<string> &line, size_t &lineNo) {
		i = line[lineNo - 1].find("\t{");
		if (i != line[lineNo - 1].npos) {
			line[lineNo - 1].replace(i, 2, "<div class=\"classes\">\t{");
		}
		else {
			i = line[lineNo - 1].find("{");
			if (i != line[lineNo - 1].npos)
				line[lineNo - 1].replace(i, 1, "<div class=\"classes\">{");
		}
	}

	//Function to handle funtions in handleSwitch function
	void Converter::handleFunction(size_t &i, vector<string> &line, size_t &lineNo) {
		i = line[lineNo - 1].find("\t{");
		if (i != line[lineNo - 1].npos) {
			line[lineNo - 1].replace(i, 2, "<div class=\"functions\">\t{");
		}
		else {
			i = line[lineNo - 1].find("{");
			if (i != line[lineNo - 1].npos)
				line[lineNo - 1].replace(i, 1, "<div class=\"functions\">{");
		}
	}

	//Function to handle one line functions opening case in handleSwitch function
	void Converter::handleOneLnFOpen(size_t &i, vector<string> &line, size_t &lineNo) {
		i = line[lineNo - 1].find("\t{");
		if (i != line[lineNo - 1].npos) {
			line[lineNo - 1].replace(i, 2, "<div class=\"functions\">\t{");
		}
		else {
			i = line[lineNo - 1].find("{");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 1, "<div class=\"functions\">{");
			}
		}
	}

	//Function to handle one line functions closing case in handleSwitch function
	void Converter::handleOneLnFClose(size_t &i, vector<string> &line, size_t &lineNo) {
		i = line[lineNo - 1].find("\t}");
		if (i != line[lineNo - 1].npos) {
			line[lineNo - 1].replace(i, 2, "\t}</div>");
		}
		else {
			i = line[lineNo - 1].find("}");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 1, "}</div>");
			}
		}
	}

	//Function to handle switch case in addDiv function
	void Converter::handleSwitch(size_t &i, vector<string> &line, std::map<std::size_t, DependencyT::TypeInfo>::iterator &iter2, size_t &lineNo) {
		switch (iter2->second) {
		case DependencyT::TypeInfo::clas:
			handleClass(i, line, lineNo);
			break;
		case DependencyT::TypeInfo::function:
			handleFunction(i, line, lineNo);
			break;
		case DependencyT::TypeInfo::oneLineFn:
			handleOneLnFOpen(i, line, lineNo);
			handleOneLnFClose(i, line, lineNo);
			break;
		case DependencyT::TypeInfo::end:
			i = line[lineNo - 1].find("\t};");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 3, "\t};</div>"); break;
			}
			i = line[lineNo - 1].find("};");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 2, "};</div>"); break;
			}
			i = line[lineNo - 1].find("\t}");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 2, "\t}</div>"); break;
			}
			i = line[lineNo - 1].find("}");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 1, "}</div>"); break;
			}
			i = line[lineNo - 1].find("\t*/");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 3, "\t*/</div>"); break;
			}
			i = line[lineNo - 1].find("*/");
			if (i != line[lineNo - 1].npos) {
				line[lineNo - 1].replace(i, 2, "*/</div>"); break;
			}break;
		default: break;
		}
	}

	//A function to add div tags at appropriate places
	void Converter::addDiv(string file, vector<string> &line, std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> &LT) {
		std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>>::iterator iter = LT.find(file);
		if (iter != LT.end()) {
			std::map<std::size_t, DependencyT::TypeInfo>::iterator iter2 = iter->second.begin();
			size_t i;
			while (iter2 != iter->second.end())
			{
				size_t lineNo = iter2->first;
				handleSwitch(i, line, iter2, lineNo);
				iter2++;
			}
		}
	}

	//A function to add Pre tags to HTML file
	void Converter::addPreTags(ofstream& webpageOutput) {
		webpageOutput << "<!Doctype html >\n";
		webpageOutput << "<html>\n";
		webpageOutput << "<head>\n";
		webpageOutput << "<link rel = \"stylesheet\" href = \"style.css\">\n";
		webpageOutput << "<script src=\"showHideClass.js\"></script>\n";
		webpageOutput << "</head>\n";
		webpageOutput << "<body>\n";
		webpageOutput << "<button onclick = \"showHideClass(\'classes\')\">Show or Hide Class Contents</button>\n";
		webpageOutput << "<button onclick = \"showHideClass(\'functions\')\">Show or Hide Function Contents</button>\n";
		webpageOutput << "<button onclick = \"showHideClass(\'comments\')\">Show or Hide Comments</button>\n";
		webpageOutput << "<button onclick = \"showHideALL(\'comments\')\">Show or Hide All</button>\n";
		webpageOutput << "<br>";
	}
	
	//A function which writes cpp code to html file and if it succeeds return true, otherwise false
	bool Converter::writeHTML(string htmlFile, vector<string> fileContent, vector<string> dep, std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> &LT) {

		ofstream webpageOutput(htmlFile);
		if (!webpageOutput.good()) {
			cout << "\n\n Unable to open the file";
			return false;
		}

		addPreTags(webpageOutput);

		if (dep.size() > 0) {
			for (size_t i = 0; i < dep.size(); i++) {
				int pos = dep[i].find_last_of("\"");
				std::string str = "\"" + dep[i].substr(0, pos) + ".html";
				webpageOutput << "<h3><a href=" + str + "\">" << dep[i] << "</a>" << "&nbsp;&nbsp;&nbsp;</h3>";
			}
		}
		else {
			webpageOutput << "<h3>This File is not dependent on any other file</h3>";
		}
		webpageOutput << "<hr>";

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
	vector<string> Converter::cpptohtml(vector<string> files, map<string, vector<string>> depTable, std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> &LT)
	{
		
		vector<string> htmlFiles;
		
		ifstream in;
		
		for (size_t i = 0; i < files.size(); i++)
		{
			vector<string> fileContent;
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

				addDiv(files[i], fileContent, LT);
				handleComments(fileContent);
				std::string name = Path::getName(files[i]);
				auto iter = depTable.find(files[i]);
				std::vector<string> dep;
				if (iter != depTable.end()) {
					for (size_t i = 0; i < iter->second.size(); i++)
						dep.push_back(iter->second[i]);
				}
				std::string htmlFile = "../../../convertedPages/" + name + ".html";
				std::string Path = Path::getFullFileSpec(htmlFile);
				htmlFiles.push_back(Path);
				
				if (!writeHTML(htmlFile, fileContent, dep, LT))
					return vector <string>();
				
				in.close();
				fileContent.clear();
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

			displayFiles(CppToHtmlFiles);
		}
		return CppToHtmlFiles;
	}

	//A function to add div tags before and after comments
	void Converter::handleComments(vector<string>& fileContent)
	{
		bool isComment = false;
		size_t found;
		for (size_t i = 0; i < fileContent.size(); i++) {
			found = fileContent[i].find("//");
			if (found != fileContent[i].npos) {
				fileContent[i].replace(found, 2, "<div class=\"comments\">//");
				fileContent[i].replace(fileContent[i].size(), fileContent[i].size(), "</div>");
			}
			found = fileContent[i].find("/*");
			if (found != fileContent[i].npos) {
				isComment = true;
				fileContent[i].replace(found, 2, "<div class=\"comments\">/*");
			}
			if (isComment) {
				size_t muLineC = fileContent[i].find("*/");
				if (muLineC != fileContent[i].npos) {
					fileContent[i].replace(muLineC, 2, "*/</div>");
					isComment = false;
				}
			}
		}
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
	
	std::cout << "\n Converter package cannot exists without dependency package and dependency package cannot exists without parser.";
	std::cout << "\n Executive shows the use of converter package in depth";

	_getche();
	return 0;
}
#endif

