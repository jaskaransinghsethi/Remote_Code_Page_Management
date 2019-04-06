//////////////////////////////////////////////////////////////////////////////////
// Translator.h  : defines a warpper for IPublisher which works with .NET code  //
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
*  This package defines Translator class which can be used by .NET applications.
*  In this, Translator is used in WPF project to allow making calls from
*  WPF C# code to Publisher native C++ code. The Translator is a mangaged (.NET)
*  wrapper around IPublisher interface which can make calls to native C++
*  classes and, in the same time, can be called by any .NET code (C# here).
*
*  Public Interface:
* ===================
* Translator trans;
* trans.startProject(argv);               //A function to start publisher in c++ native code. Argv is List<String> from WPF application
* trans.returnConvertedF(files)			  //A function to return list of converted files from C++ native code back to WPF application
*
*  Required Files:
* =================
*  Translator.h Translator.cpp IPublisher.h
*
*  Build Command:
* ================
*  msbuild translator.vcxproj
*
*  Maintainence History:
* =======================
*  ver 1.0 - March 30th 2019
*    - first release
*
*/
#pragma once

#include <string>
#include "../Publisher/IPublisher.h"

using namespace System;
using namespace System::Collections::Generic;

//Managed Translator class for WPF application
public ref class Translator
{
public:
	Translator();
	~Translator();
	List<String^>^ startProject(List<String^>^ argv);
	List<String^>^ returnConvertedF(const std::vector<std::string> &files);
private:
	Ipublisher* client;
	std::string sysStrToStdStr(String^ str);
	String^ stdStrToSysStr(const std::string& str);
};

