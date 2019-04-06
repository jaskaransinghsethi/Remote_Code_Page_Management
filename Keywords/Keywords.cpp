#include "Keywords.h"
#include<sstream>
#include<iostream>
#include<string>



Keywords::Keywords()
{
}


Keywords::~Keywords()
{
}

void Keywords::createMap()
{
	std::ifstream in;
	in.open("keywords.txt");
	if (!in.good()) {
		std::cout << "\n\n Unable to Open the file";
		return;
	}
	for (std::string line; std::getline(in, line); )
	{
		auto find = keywords_.find(line);
		if (find == keywords_.end()) {
			keywords_.insert(std::pair<std::string, int>(line, 1));
		}
	}
}

std::vector<std::string> Keywords::handleKeywords(std::vector<std::string> fileContent) {
	std::string word;
	std::string line;
	std::vector<std::string> Content;
	for (size_t i = 0; i < fileContent.size(); i++) {
		std::stringstream in(fileContent[i]);
		try {
			while (in >> word)
			{
				auto iter = keywords_.find(word);
				if (iter != keywords_.end()) {
					word.replace(0, word.size(), "<font class=\"comments\">" + word + "</>");
				}
				line += word + " ";
				auto i = in.peek();
				if (i == -1) {
					line += "\n";
				}
			}
			Content.push_back(line);
			line.clear();
		}

		catch (std::exception& e) {
			std::cout << "\n Error opening/reading/writing to the file. \n Exception:-  " << e.what();
		}
	}
	return Content;
}
