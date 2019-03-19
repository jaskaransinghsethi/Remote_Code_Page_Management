#pragma once
///////////////////////////////////////////////////////////////////////////////////////
// DependencyT.h - DependencyT package creates dependency table				         //                  
// ver 1.1                                                                           //
// Jaskaran Singh, CSE687 - Object Oriented Design, Spring 2018                      //
///////////////////////////////////////////////////////////////////////////////////////
/*
*
*Package Operations:
* -------------------
* DependencyT package takes files from the DirectoryExplorerN and parse them, finding
* all the dependencies of the given file. It also creates Line which records the
* start line count and end line count of functions, classes, structs, interfaces
* and comments
*
* Public Interface:
* =================
* DependencyT dep;												//To create an object of DependencyT class 
* dep.callParser(std::vector<std::string> files);				//To call parser to parse given set of files 
* dep.createDepT(pGlobalScope, depTable, files, path, fileSpec);//To create dependency table of a given set of files
* enum TypeInfo { none, clas, function, end, comment };			//Enumerator to set the TypeInfo of a given node
* findTypeInfo(CodeAnalysis::ASTNode* pGlobalScope);			//To detemine the type of given node int AST
* returnLT();													//Return line table
* manipulateDepTable(depTable, files);							//To trim relative path into file name
* createLT(pGlobalScope, file);									//Reads Abstract Syntax Tree and create line table
*																if a given node is comment, function or a class
*iChecks(pParser, files[i], fileSpec, name, configure);			//Function to perform intial checks before actually running parser
*
*runParser(files, depTable, fileSpec, path);					//Function to actually run parser using Parser, configureparser and
*																and rulesandaction classes
*Function for initial checks before actually running parser
/*Required Files:
* ---------------
* Parser.h					//Parser class
* ConfigureParser.h			//ConfigureParser class
* ActionAndRules.h			//Rules and Actions
* ScopeStack.h				//Repository and ScopeStack
* AbstrSynTree.h			//Abstract Syntax tree class
* Utlities.h				//Util class
*
*Maintenance History:
* --------------------
*  ver 1.1 : 28 Feb 2019
* -created depTable, enum TypeInfo and LineTable
* ver 1.0 : 27 Feb 2019
* - first release
*/
#include<map>
#include<vector>
#include<string>
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Utilities/Utilities.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"

class DependencyT
{
public:
	DependencyT();
	~DependencyT();
	enum TypeInfo { none, clas, function, end, oneLineFn };
	std::map<std::string, std::map<std::size_t, TypeInfo>> returnLT() { return LT; }
	void manipulateDepTable(std::map<std::string, std::vector<std::string>> &depTable, std::vector<std::string> files);
	void createLT(CodeAnalysis::ASTNode* pGlobalScope, std::string file);
	std::map<std::string, std::vector<std::string>> callParser(std::vector<std::string> files);
	TypeInfo findTypeInfo(CodeAnalysis::ASTNode* pGlobalScope);
	void createDepT(CodeAnalysis::ASTNode* pGlobalScope, std::map<std::string, std::vector<std::string>> &depTable, std::vector<std::string> files, std::string path, std::string fileSpec);
	bool iChecks(CodeAnalysis::Parser * pParser, std::string & file, std::string & fileSpec, std::string & name, CodeAnalysis::ConfigParseForCodeAnal & configure);
	bool runParser(std::vector<std::string>& files, std::map<std::string, std::vector<std::string>>& depTable, std::string & fileSpec, std::string & path);
private:
	std::map<std::string, std::map<std::size_t, DependencyT::TypeInfo>> LT;
};

