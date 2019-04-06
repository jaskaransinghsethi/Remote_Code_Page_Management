#pragma once
#include<map>
#include<vector>
#include<fstream>
class Keywords
{
private:
	std::map<std::string, int> keywords_;
public:
	Keywords();
	~Keywords();
	void createMap();
	std::vector<std::string> handleKeywords(std::vector<std::string> fileContent);
	std::map<std::string, int> returnMap() { createMap(); return keywords_; }
};

