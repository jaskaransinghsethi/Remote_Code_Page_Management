//////////////////////////////////////////////////////////////////////////////////
// IPublisher.h  : Provides an interface to a working C++ Publisher class	    //
// ver 1.0																		//
//																				//
// Application    : CSE-687 Project 3					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
*  This package defines a C++ class that uses <thread>, Cpp11-BlockingQueue
*  which uses <condition variable> and <mutex>. These concurrency classes
*  are not supported in CLR mode and cannot compile under /CLR flag.
*  Therefore, the implementation of the class was moved to the .cpp file
*  leaving the .h file free of any CLR-incompatible types. However, we
*  still need to use the C++ class in C++/CLI environment and under /CLR
*  code. To do so, we created an interface (ICppClass shown below) that
*  declares the methods we want the C++/CLI Shim to use. The concept of
*  inheritance allows the use of an interface pointer (ICppClass*) to
*  point into any derived (or implementation) class objects (CppClass).
*  Therefore, we declared an object factory that returned ICppClass*
*  and implemented the function in the .cpp file to return CppClass*.
*  This way, the C++/CLI Shim will be able to use CppClass through
*  its interface without any knowledge about it and hence CppClass'
*  CLR incompatible types are of no issue anymore.
*
*  The ObjectFactory (shown below) can be used without the 'extern "C"'
*  marker but it is not recommended to do so. The marker will prevent
*  the compiler from doing any namemangeling on the function allowing
*  the use of PInvoke if needed. For this application it doesn't make
*  a difference so you can use either.
*
*
*  Public Interface:
* ===================
*  ObjectFactory factory;
*  IPublisher* instance = factory.createClient();
*  instance->startProject(argc, argv);
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