//////////////////////////////////////////////////////////////////////////////////
// Translator.cpp  : provides definition and test stub for Translator.h			//
// ver 1.0																		//
//																				//
// Application    : CSE-687 Project 3					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Translator.h"
#include <iostream>

//Default Constructor
Translator::Translator()
{
	Console::Write("\n  Translator created");
	ObjectFactory factory;
	client = factory.createClient();
	Console::Write("\n  Translator instantiated Publisher as IPublisher*");
}

//Default destructor
Translator::~Translator()
{
	delete client;
	Console::Write("\n  Translator destroyed\n\n");
}

//A function to start publisher in c++ native code. Argv is List<String> from WPF application
List<String^>^ Translator::startProject(List<String^>^ args)
{
	int argc = args->Count;
	char **argv = new char*[argc];
	for (int i = 0; i < argc; i++) {
		std::string arg = sysStrToStdStr(args[i]);
		argv[i] = new char[args[i]->Length]();
		strcpy_s(argv[i], arg.length()+1, arg.c_str());
	}
	std::vector<std::string> temp = client->startProject(argc, argv);
	return returnConvertedF(temp);
}

//A function to return list of converted files from C++ native code back to WPF application
List<String^>^ Translator::returnConvertedF(const std::vector<std::string> &files) {
	List<String^>^ convertedF = gcnew List<String^>;
	for (size_t i = 0; i < files.size(); i++) {
		String^ cFile = stdStrToSysStr(files[i]);
		convertedF->Add(cFile);
	}
	return convertedF;
}

//A function to convert System::String to STD::String
std::string Translator::sysStrToStdStr(String ^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
}

//A function to convert STD::String to System::String
String ^ Translator::stdStrToSysStr(const std::string & str)
{
	return gcnew String(str.c_str());
}

/*-----< Shim Test Stub >---------------------------------------------*/
#ifdef TEST_TRANSLATOR

int main(array<System::String^> ^args) {
	Translator trans;
	List<String^>^ arg = gcnew List<String^>;
	arg->Add("useless");
	arg->Add("..");
	arg->Add("/s");
	arg->Add("*.h");
	arg->Add("*.cpp");
	arg->Add("[A-B](.*)");
	trans.startProject(arg);

	return 0;
} // NOTE: Shim destructor will automatically be called here just like in native C++

#endif

