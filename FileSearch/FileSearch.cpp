#include "FileSearch.h"

///////////////////////////////////////////////////////////////////
//  FileSearch.cpp   -  implementation file for Filesearch class //
//  ver 2.1                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:  Project - 4(Remote Code Management)            //
//  Author:       Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////


//Takes display info and map(where files and path iterators are stored) as arguments
//and generates the absolute path and controls he operations of other functions
//like fileSearch and filterFiles.
std::string FileSearch::findText(std::string filePaths, std::string searchString)
{
	
	BOOLEAN booleanFileFound = false;
	BOOLEAN  booleanAnyFileFound = false;
	std::string specificFile;
	std::string files = "";
	std::vector<std::string> vec;
	if (filePaths.find("|") != std::string::npos)
		vec = split(filePaths, '|');
	else
		vec.push_back(filePaths);
	for (auto file : vec)
	{
		specificFile = filterFiles(patternsVector, file);
		booleanFileFound = fileSearch(searchString, specificFile);
		if (booleanFileFound)
		{
			booleanAnyFileFound = true;
			if (files != "")
			{
				files = files + "|" + file;
			}
			else
			{
				files = file;
			}
			//dp_.outputSearchedFileResults(file);
		}
	}
	if (booleanAnyFileFound == false)
	{
		files = "";
	}
	return files;
}

std::vector<std::string> FileSearch::split(std::string filePaths, char separator)
{
	std::vector<std::string> elements;
	std::stringstream ss(filePaths);
	std::string item;
	while (std::getline(ss, item, separator)) {
		elements.push_back(item);
	}
	return elements;
}

//filter the files according to pattern mentioned.
std::string FileSearch::filterFiles(std::vector<std::string> strPatternsandText_,std::string file){
	std::string specificFile;
	std::string tempFilePattern = file;
	if (strPatternsandText_.size() > 0)
	{ 
		//filter the files according to pattern entered by user
		for (auto element : strPatternsandText_)
		{
			tempFilePattern = file.substr(file.find_last_of('.'), file.size());
			//no need to look into pattern if the pattern is *.*
			if (element == ".*")
			{
				specificFile = file;
				return specificFile;
			}
			if (tempFilePattern == element)
			{
				specificFile = file;
			}
		}
	}
	else
	{
		specificFile = file;
	}
	return specificFile;
}


BOOLEAN FileSearch::fileSearch(std::string stringToBeSearched, std::string file)
{
	BOOLEAN boolSearch = false;
	std::fstream myFile2;
	myFile2.open(file, std::fstream::in);
	int offset;
	std::string line;
	if (myFile2.is_open())
	{
		while (!myFile2.eof())
		{
			getline(myFile2, line);
			if ((offset = line.find(stringToBeSearched, 0)) != std::string::npos)
			{
				boolSearch = true;
			}
		}
	}
	return boolSearch;
}



#ifdef TEST_FILESEARCH
int main()
{
	FileSearch fs;
	std::string list = fs.findText("..//FileSearch/FileSearch.cpp","FileSearch");
	std::cout << "Text: 'FileSearch' is found in: " << "\n";
	std::vector<std::string> vec;
	if (list.find("|") != std::string::npos)
		vec = fs.split(list, '|');
	else
		vec.push_back(list);
	for (auto elem : vec)
		std::cout << elem << "\n";

	return 0;
}

#endif