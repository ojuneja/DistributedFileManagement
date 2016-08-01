

///////////////////////////////////////////////////////////////////
//  FileManager.cpp  - Implementation file for FileManager class //
//  ver 2.1                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:  Project - 4(Remote Code Management)            //
//  Author:       Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////


#include <iostream>
#include "..//FileSystem/FileSystem.h"
#include "FileManager.h"

FileManager::FileManager(const std::string& path, std::vector<std::string>& vector, FileSearch &fs) :path_(path), optionsVector_(vector), fs_(fs)
{
	patterns_.push_back("*.*");
}

//<-------set path.---->
void FileManager::resetPath(std::string path)
{
	std::string temp = FileSystem::Path::getFullFileSpec(path);
	FileSystem::Directory::setCurrentDirectory(temp);
	//std::cout << FileSystem::Directory::getCurrentDirectory() << std::endl;
}

//Acts as entry point function for all functions.
//allow other functions to perform its task if its flag is
//true and returns display information.
std::string FileManager::search() {
	processInput();
	std::string temp = FileSystem::Path::getFullFileSpec(".");
	BOOLEAN pathExitsFlag = false;
	if (invalidFlag)
	{
		displayString = "Invalid";
	}
	else{
		// checking whether the path entered exists or not
		pathExitsFlag = FileSystem::Directory::exists(path_);
		path_ = FileSystem::Path::getFullFileSpec(path_);
		if (pathExitsFlag)
		{
			if (subdirectoriesFlag)
			{
				for (auto patt : patterns_) {
					findDirectoriesAndSubdirectories(path_, patt);
				}
			}
		}
		else
		{
			displayString = "Invalid";
		}
	}
	clearOptionandPatternsVector();
	resetflags();
	resetPath(temp);
	if (store_ == "")
	{
		store_ = "";
	}
	return store_;
}

//Process input and dissects valid options(/ s), patterns
//and then stores in particular function or in some container.
void FileManager::processInput()
{
	int pos = 0;
	//Iterating through options vector(which contains options and patterns) and storing valid patterns and options(/d /s /f) in proper vectors.
		for (processIterator = optionsVector_.begin(); processIterator != optionsVector_.end(); ++processIterator)
		{
			std::string option = "";
		 if (std::regex_match(*processIterator, std::regex("(/)(.*)")))
			{
				listOfOptions.push_back(*processIterator);
			}
			else if ((pos = (*processIterator).find("*.")) != std::string::npos)
			{
				if (*processIterator == "*.*") //clearing the patterns if *.* is encountered.
				{
					patterns_.clear();
					patterns_.push_back("*.*");
					NoPushFlag = true;
				}
				addFilePattern(*processIterator);
			}
			else
			{
				invalidFlag = true;
			}
		}
		if (!invalidFlag)
		{
			if (listOfOptions.size() == 0)
			{
				//setting directory flag is true if no option is provided by user
				displayString = displayString + "_directory";
			}
			else
				processOptionsAndPatterns(listOfOptions);
		}
	}


//Process options(/ s) and sets appropriate flags as
//true.Also specifies display info needed for display package.
void FileManager::processOptionsAndPatterns(list listOfOptions)
{
	//setting the flag as true according to the options provided
		for (auto elem : listOfOptions)
		{
			if (elem == "/s" || elem == "/S")
			{
				subdirectoriesFlag = true;
				displayString = displayString + "_subdirectories";
				
			}
			else
			{
				invalidFlag = true;
			}
		}
	}



//process and add patterns to vector
void FileManager::addFilePattern(const std::string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == ("*.*") && NoPushFlag == false)  {
		patterns_.pop_back(); //popping the pattern(which is pushed in constructor) if pattern is mentioned by user.
		patterns_.push_back(patt);
	}
	else if (patterns_.size() >= 1 && patterns_[0] != ("*.*"))
	{
		patterns_.push_back(patt); 
	}
}


//Accepts path and pattern as arguments and search files
//in the directories and subdirectories present in that path
//and develops catalog by saving in string.
void FileManager::findDirectoriesAndSubdirectories(const std::string &path, const std::string patt) {
	std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
	std::vector<std::string> ::iterator iterDirectory;
	std::vector<std::string> directories = FileSystem::Directory::getDirectories(path);
	FileSystem::Directory::setCurrentDirectory(path);
	if (directories.size() != 0)
	{
		directories.erase(directories.begin(), directories.begin() + 2);
		for (iterDirectory = directories.begin(); iterDirectory != directories.end(); ++iterDirectory)
		{
			*iterDirectory = FileSystem::Path::fileSpec(FileSystem::Directory::getCurrentDirectory(), *iterDirectory);
		}
	}
	if (files.size() != 0)
	{
		for (auto iteration : files)
		{
			if (store_ != "")
			{
				store_ = store_ + "|" + FileSystem::Path::getFullFileSpec(iteration);
			}
			else
			{
				store_ = FileSystem::Path::getFullFileSpec(iteration);
			}
			
		}
	}
	if (directories.size() != 0)
	{
		for (auto dir : directories)
		{
			//recursive call of function to iteratae through sub directories
			findDirectoriesAndSubdirectories(dir, patt);
		}
	}
}



void FileManager::clearOptionandPatternsVector()
{
	listOfOptions.clear();
	optionsVector_.clear();
	patterns_.clear();
}

//resets all flags as false
void FileManager::resetflags()
{
	subdirectoriesFlag = false;
    invalidFlag = false;
	 NoPushFlag = false;
}



#ifdef TEST_FILEMANAGER
int main() {
	FileSearch fs;
	int i = 0;
	std::string displayString;
	std::vector<std::string> testVector;
	std::cout << "\n""\n""\n"<<"Test Case 1: path is current directory , Pattern/Patterns is/are: *.h *.cpp and option is /s" << "\n\n";
	testVector.push_back("*.h");
	testVector.push_back("*.cpp");
	testVector.push_back("/s");
	FileManager fm("../", testVector,fs);
	displayString = fm.search();
	std::vector<std::string> elements;
	std::stringstream ss(displayString);
	std::string item;
	std::cout << "Searched Files are:"<<"\n";
	while (std::getline(ss, item, '|')) {
		elements.push_back(item);
		std::cout << elements[i] << "\n";;
		i ++ ;
	}	
	return 0;
}
#endif