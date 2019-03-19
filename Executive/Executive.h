#pragma once
//////////////////////////////////////////////////////////////////////////
// Executive.h - Executive for project 1                                //                  
// ver 1.1                                                             //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////
/*
*
*Package Operations :
*------------------ -
*Executive package is created to execute all the functionalities implemented
*in the project.
*
* Public Interface:
* =================
* Executive exe;											//To create an object of executive class
* exe.runProject(Utilities::ProcessCmdLine& cmdargs);		//To execute runProject() function to show all the functionalities of the project
* exe.Req4();												//To show Requirement 4 in the project
*
*Required Files :
*-------------- -
*Executive.h, Executive.cpp				//Executive class
*Converter.h, Converter.cpp				//Converter class
*Display.h, Display.cpp					//Display class
*CodeUtilities.h, CodeUtilities.cpp		//CodeUtilities class
*StringUtilities.h, StringUlities.cpp	//StringUtilities class
*DirExplorerN.h, DirExplorerN.cpp		//DirExplorer-Naive Class
*
*Maintenance History :
*--------------------
* - Created runProject and Req4 functions
* ver 1.1 : 2 Feb 2019 
* -Created Executive class
* ver 1.0 : 26 Jan 2019
* -first release
*/

#include "../CodeUtilities/CodeUtilities.h"

class Executive
{
public:
	Executive();
	~Executive();
	void runProject(Utilities::ProcessCmdLine& cmdargs);
	void Req4();
};

