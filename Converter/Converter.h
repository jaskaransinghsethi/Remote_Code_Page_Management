#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Converter.h - Converter class to convert cpp files into html files            //
// ver 1.3                                                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018                       //
///////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 *Converter provides a class, of the same name, that has 3 main functions, first,
 *cpptohtml which takes a vector of string of files to be converted into html files
 *and returns a vector of string of converted html files.
 *Second, initialChecks function which takes the object of ProcessCmdLine class
 *and which call DirectorExlorer-N to returns all the directories and subdirectories
 *within directories and files with the matching pattern. initialChecks returns the vector
 *of string of files which are to be converted to html files.
 *Third, addDiv tags which add div tags to the appropriate place, which uses
 *helper functions to achieve this
 *
 * Public Interface:
 * =================
 * Converter conv;														//To create object of converter class
 * vector<string> files = conv.initialChecks(ProcessCmdLine obj);		//To retrieve all the files from a given directory
 * vector<string> htmlFiles = conv.cpptohtml(vector<string> files);		//To convert all the given files into valid html files 
 *																		  and return their valid html file name
 * bool success = writeHTML(string htmlFile, vector<string> fileContent); //To write cpp code to html file and returns true if success, 
 *																			otherwise false
 * display(Files);														//Function to display a list of files to be processed
 * addDiv(files, fileContent, LT);										//Function to add div tags at appropriate places in classes and functions
 * addPreTags(webpageOutput);											//Function to add pre tags into the HTML file
 * handleComments(fileContent);											//Function to add div tags before and after comments in the html file
 * handleSwitch(i,line,iter2,lineNo);									//Helper function for addDiv function
 * handleFunction(i,line,lineNo);										//Helper function for handleSwitch function
 * handleClass(i,line,lineNo);											//Helper function for handleSwitch function
 * handleOneLnFOpen(i, line, lineNo);									//Helper function for handleSwitch function
 * handleOneLnFClose(i, line, lineNo);									//Helper function for handleSwitch function
 *
 *Required Files:
 * ---------------
 * DirExplore-N.h, DirExplorer-N.cpp  //Director Explorer Naive
 * FileSystem.h, FileSystem.cpp       // Directory and Path classes
 * StringUtilities.h                  // Title function
 * CodeUtilities.h                    // ProcessCmdLine class
 * DependencyT.h					  // Dependencies class
 *
 * Maintenance History:
 * --------------------
 * ver 1.2 : 5 March 2019
 * - Made adddiv, addpretags, handlecomments functions
 * ver 1.1 : 28 Jan 2019
 * - Made initialChecks, cpptohtml funtions, display functions
 * ver 1.0 : 26 Jan 2019
 * - first release
 */

#include<string>
#include<vector>
#include "../CodeUtilities/CodeUtilities.h"
#include<map>
#include "../DependencyT/DependencyT.h"

namespace html {
	using namespace std;
	using namespace Utilities;

	class Converter
	{
	public:
		Converter();
		~Converter();
		void addDiv(string file, std::vector<std::string> &line, std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> &LT);
		void addPreTags(ofstream & webpageOutput);
		void displayFiles(vector<string> Files);
		bool writeHTML(string htmlFile, vector<string> fileContent, vector<string> depTable, std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> &LT);
		vector<string> cpptohtml(vector<string> files, map<string, vector<string>> depTable, std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> &LT);
		vector<string> initialChecks(ProcessCmdLine &pcl);
		void handleComments(vector<string> &fileContent);
		void handleSwitch(size_t &i, vector<string> &line, std::map<std::size_t, DependencyT::TypeInfo>::iterator &iter2, size_t &lineNo);
		void handleFunction(size_t &i, vector<string> &line, size_t &lineNo);
		void handleOneLnFOpen(size_t & i, vector<string>& line, size_t & lineNo);
		void handleOneLnFClose(size_t & i, vector<string>& line, size_t & lineNo);
		void handleClass(size_t &i, vector<string> &line, size_t &lineNo);
	};

}