//////////////////////////////////////////////////////////////////////////////////
// Executive.cpp  : Provides a test stub for object factory and Ipublisher	    //
// ver 1.0																		//
//																				//
// Application    : CSE-687 Project 3					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////

#include "IPublisher.h"
#include <iostream>
#include "Publisher.h"

using namespace std;

//Test stub for Ipublisher & Publisher
#ifdef TEST_EXECUTIVE
int main(int argc, char **argv) {
	ObjectFactory factory;
	Ipublisher* instance = factory.createClient();
	instance->startProject(argc, argv);
	getchar();
}
#endif