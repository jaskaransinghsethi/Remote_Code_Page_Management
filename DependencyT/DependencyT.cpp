///////////////////////////////////////////////////////////////////////////////////////
// DependencyT.h - DependencyT package creates dependency table				         //                  
// ver 1.1                                                                           //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018                      //
///////////////////////////////////////////////////////////////////////////////////////

#include "DependencyT.h"
#include <algorithm>

//Namespaces
using namespace CodeAnalysis;
using namespace Utilities_P;

//Default Constructor
DependencyT::DependencyT()
{
}

//Default Destructor
DependencyT::~DependencyT()
{
}

//function to find TypeInfo
DependencyT::TypeInfo DependencyT::findTypeInfo(ASTNode* pGlobalScope)
{
	if (pGlobalScope->type_ == "namespace")
		return none;
	if (pGlobalScope->type_ == "class")
		return clas;
	if (pGlobalScope->type_ == "function" && (pGlobalScope->endLineCount_ - pGlobalScope->startLineCount_ == 0))
		return oneLineFn;
	if (pGlobalScope->type_ == "function")
		return function;
	if (pGlobalScope->type_ == "interface")
		return function;
	if (pGlobalScope->type_ == "struct")
		return clas;

	return none;

}

//function to create LineTable which records if a given node in AST is a comment or function or class
void DependencyT::createLT(ASTNode* pGlobalScope, std::string file) {
	static size_t indentLevel = 0;
	TypeInfo type = findTypeInfo(pGlobalScope);
	std::map<size_t, TypeInfo> temp;
	auto name = file;
	if (pGlobalScope->package_ != "" && pGlobalScope->type_ != "control" && pGlobalScope->type_!="anonymous") {
		temp.insert(std::pair<size_t, TypeInfo>(pGlobalScope->startLineCount_, type));
		if (pGlobalScope->type_ == "class" || pGlobalScope->type_ == "struct") {
			temp.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_-1, end));
		}
		else {
			temp.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_, end));
		}
		auto find = LT.find(file);
		if (find == LT.end()) {
			LT.insert(std::pair<std::string, std::map<size_t, TypeInfo>>(name, temp));
		}
		else {
			find->second.insert(std::pair<size_t, TypeInfo>(pGlobalScope->startLineCount_, type));
			if (pGlobalScope->type_ == "class" || pGlobalScope->type_ == "struct")
				find->second.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_ - 1, end));
			else
				find->second.insert(std::pair<size_t, TypeInfo>(pGlobalScope->endLineCount_, end));
		}
	}
	auto iter = pGlobalScope->children_.begin();
	++indentLevel;

	while (iter != pGlobalScope->children_.end())
	{
		createLT(*iter, file);
		++iter;
	}
	--indentLevel;
}

//A function to create dependency table
void DependencyT::createDepT(ASTNode* pGlobalScope, std::map<std::string, std::vector<std::string>> &depTable, std::vector<std::string> files, std::string path, std::string fileSpec) {
	auto iter2 = pGlobalScope->statements_.begin();
	while (iter2 != pGlobalScope->statements_.end()) {
		std::string str = (*iter2)->ToString();
		int pos = str.find_first_of("\"");
		int posi = str.find_last_of("\"");
		std::string dep = str.substr(pos + 1, posi - pos - 1);
		std::replace(dep.begin(), dep.end(), '/', '\\');
		std::string filePath = path + dep;
		std::string fullyqualified = FileSystem::Path::getFullFileSpec(filePath);
		auto found = std::find(files.begin(), files.end(), fullyqualified);
		if (found != files.end()) {
			auto iter = depTable.find(fileSpec);
			if (iter == depTable.end()) {
				std::vector<std::string> temp;
				temp.push_back(dep);
				depTable.insert(std::pair<std::string, std::vector<std::string>>(fileSpec, temp));
			}
			else
				iter->second.push_back(dep);
		}
		++iter2;
	}
}

//Function for initial checks before actually running parser
bool DependencyT::iChecks(CodeAnalysis::Parser* pParser, std::string &file, std::string &fileSpec, std::string &name, CodeAnalysis::ConfigParseForCodeAnal &configure) {
	if (pParser)
	{
		name = FileSystem::Path::getName(file);
		if (!configure.Attach(fileSpec))
		{
			std::cout << "\n  could not open file " << name << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "\n\n  Parser not built\n\n";
		return 0;
	}

	return 1;
}

//function to run parser
bool DependencyT::runParser(std::vector<std::string> &files, std::map<std::string, std::vector<std::string>> &depTable, std::string &fileSpec, std::string &path) {
	for (size_t i = 0; i < files.size(); ++i)  // iterate over files
	{
		fileSpec = FileSystem::Path::getFullFileSpec(files[i]);
		path = FileSystem::Path::getPath(fileSpec);

		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();

		std::string name;

		try
		{
			if (!iChecks(pParser, files[i], fileSpec, name, configure))
				return 0;

			Repository* pRepo = Repository::getInstance();
			pRepo->package() = name;

			while (pParser->next())
			{
				pParser->parse();
			}

			ASTNode* pGlobalScope = pRepo->getGlobalScope();

			complexityEval(pGlobalScope);

			createLT(pGlobalScope, files[i]);

			createDepT(pGlobalScope, depTable, files, path, fileSpec);
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
	}
	return 1;
}

//function to call parser on the set of files
std::map<std::string, std::vector<std::string>> DependencyT::callParser(std::vector<std::string> files) {
#define Util StringHelper
	std::string fileSpec;
	std::string path;
	std::map<std::string, std::vector<std::string>> depTable;

	if (!runParser(files, depTable, fileSpec, path))
		return std::map<std::string, std::vector<std::string>>();

	return depTable;
}

//function to trim path into file name and store into depTable
void DependencyT::manipulateDepTable(std::map<std::string, std::vector<std::string>> &depTable, std::vector<std::string> files) {
	if (depTable.size() == 0)
		return;

	auto iter = depTable.begin();
	while (iter != depTable.end()) {
		for (size_t j = 0; j < iter->second.size(); j++) {
			size_t pos = iter->second[j].find_last_of("\\");
			if (pos != iter->second[j].npos) {
				iter->second[j] = iter->second[j].substr(pos + 1, iter->second[j].size());
			}
		}
		iter++;
	}
}

//Test stub for dependency package
#ifdef TEST_DEP

int main(int argc, char* argv[])
{
	if (argc < 2 && argc > 11) {
		std::cout << "Invalid commandline arguments";
		return -1;
	}

	std::vector<std::string> files = { argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11], };
	std::map<std::string, std::vector<std::string>> depTable;
	DependencyT dep;

	for (size_t i = 0; i < files.size(); i++) {
		std::string fileSpec = FileSystem::Path::getFullFileSpec(files[i]);
		files[i] = fileSpec;
	}

	depTable = dep.callParser(files);
	dep.manipulateDepTable(depTable, files);
	auto iter = depTable.begin();
	while (iter != depTable.end()) {
		std::cout << "\n File:" << iter->first;
		std::cout << "\n Dependencies:";
		for (size_t i = 0; i < iter->second.size(); i++) {
			std::cout << iter->second[i]<<"\t";
		}
		std::cout << "\n";
		iter++;
	}
	getchar();
	return 0;
}
#endif

