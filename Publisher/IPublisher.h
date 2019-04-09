//////////////////////////////////////////////////////////////////////////////////
// IPublisher.h  : Provides an interface to a working C++ Publisher class	    //
// ver 1.0																		//
//																				//
// Application    : CSE-687 Project 3					                        //
// Platform       : Visual Studio 2017					                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
* IPublisher interface is implemented by the Publisher. It is created to hide the
* the details of Publisher from C++ CLR, Translator. It supports the interaction of WPF
* GUI with native code.
*
*  Public Interface:
* ===================
*  ObjectFactory factory;								//Create an instance of object factory
*  IPublisher* instance = factory.createClient();		//Create an instance of Publisher and return a pointer of IPublisher type
*  instance->startProject(argc, argv);					//A function to execute all the functionalities of the Publisher
*
*  Required Files:
* =================
*  IPublisher.h Publisher.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - March 30th 2019
*  - first release
*/

#pragma once

#include<string>
#include<vector>

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

//An Interface to interact with WPF Application through CLR
//Publisher implements the IPublisher
class Ipublisher {
public:
	virtual std::vector<std::string> startProject(int argc, char** argv) = 0;
	virtual ~Ipublisher() {};
};

//Defining object factory in C code
extern "C" {
	// Instantiates Publisher class as IPublisher*
	struct ObjectFactory {
		DLL_DECL Ipublisher* createClient();
	};
}