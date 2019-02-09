#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Converter.h - Converter class to convert cpp files into html files            //
// ver 1.3                                                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018                       //
///////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 *Converter provides a class, of the same name, that has two functions, first,
 *cpptohtml which takes a vector of string of files to be converted into html files
 *and returns a vector of string of converted html files.
 *Second, initialChecks function which takes the object of ProcessCmdLine class
 *and which call DirectorExlorer-N to returns all the directories and subdirectories
 *within directories and files with the matching pattern. initialChecks returns the vector
 *of string of files which are to be converted to html files.
 * 
 * Public Interface:
 * =================
 * Converter conv;                    //To create object of converter class
 * vector<string> files = conv.initialChecks(ProcessCmdLine obj);   //To retrieve all the files from a given directory
 * vector<string> htmlFiles = conv.cpptohtml(vector<string> files); //To convert all the given files into valid html files 
 *																	  and return their valid html file name
 * bool success = writeHTML(string htmlFile, vector<string> fileContent); //To write cpp code to html file and returns true if success, 
 *																			otherwise false
 * display(Files);	//Function to display a list of files to be processed
 *
 *Required Files:
 * ---------------
 * DirExplore-N.h, DirExplorer-N.cpp  //Director Explorer Naive
 * FileSystem.h, FileSystem.cpp       // Directory and Path classes
 * StringUtilities.h                  // Title function
 * CodeUtilities.h                    // ProcessCmdLine class
 *
 * Maintenance History:
 * --------------------
  * - Made initialChecks, cpptohtml funtions, display functions
 * ver 1.0 : 26 Jan 2019
 * - first release
 */

#include<string>
#include<vector>
#include "../CodeUtilities/CodeUtilities.h"

namespace html {
	using namespace std;
	using namespace Utilities;

	class Converter
	{
	public:
		Converter();
		~Converter();
		void displayFiles(vector<string> Files);
		bool writeHTML(string htmlFile, vector<string> fileContent);
		vector<string> cpptohtml(vector<string> files);
		vector<string> initialChecks(ProcessCmdLine &pcl);
	};

}