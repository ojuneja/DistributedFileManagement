#ifndef FILEMANAGER_H
#define FILEMANAGER_H

///////////////////////////////////////////////////////////////////
//  FileManager.h      -  header file for FileManager class      //
//  ver 2.1                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:  Project - 4(Remote Code Management)            //
//  Author:       Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

/*
Package Operations:
=================
This package performs operations like processing inputs and dissect valid inputs 
properly and also search files in directories and subdirectories.
The package uses flag approach.
All the options and patterns are processed first and then flags are set and display
information needed for display package is stored.
Then,particular operation is performed only if its flag is true.
The operations are performed then and appropriate files and paths are stored in datastore.
This package is responsile for developing searching in directories and developing file catalog.


Public Interface:
=================
void addFilePattern(const std::string& patt)                                                 - process and add patterns to vector
void findDirectoriesAndSubdirectories(const std::string &path,const std::string pattern)     - accepts path and pattern as arguments and search files 
                                                                                               in the directories and subdirectories present in that path 
																							   and develops catalog by saving in string.
std::string search                                                                           - acts as entry point function for all functions.
                                                                                               allow other functions to perform its task if its flag is
																							   true and returns display information.
void processInput                                                                            - process input and dissects valid options(/s ),patterns
                                                                                               and then stores in particular function or in some container.
void processOptionsAndPatterns(list)                                                         - process options(/s ) and sets appropriate flags as 
                                                                                               true.Also specifies display info needed for display package. 
void clearOptionandPatternsVector                                                            - resets options and patterns vector
void resetflags                                                                              - resets all flags as false

*/
/*Required Files :
*---------------- -
* FileSystem.h , FileSystem.cpp , FileSearch.h , FileSearch.cpp , FileManager.h , FileManager.cpp
*
*  Build Command :
*----------------
*    devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*----------------------
* ver 2.1 : 26 Apr 2016 -  removed some functions
*Ver 2.0 : 9 Feb 16
* -second release
*Ver 1.0 : 6 Feb 16
* -first release
*/


#include<vector>
#include<iostream>
#include"..//FileSystem/FileSystem.h"
#include"..//FileSearch/FileSearch.h"
#include<fstream>
#include<regex>
#include<sstream>

class FileManager{

public:
	using list = std::list < std::string > ;
	using vector = std::vector < std::string >;
	using File = std::string;
	using patterns = std::vector < std::string > ;
	using options = std::vector < std::string > ;
	
	void addFilePattern(const std::string& patt);
	void findDirectoriesAndSubdirectories(const std::string &path,const std::string pattern);
	std::string search();
	FileManager(const std::string& path,std::vector<std::string>& vector,FileSearch &fs);
	void processInput();
	void processOptionsAndPatterns(list);
	void clearOptionandPatternsVector();
	void resetflags();
	void resetPath(std::string path);
private:
	list listOfOptions;
	std::string path_;
	patterns patterns_;
	options& optionsVector_;
	vector::iterator processIterator;
	BOOLEAN subdirectoriesFlag = false;
	BOOLEAN invalidFlag = false;
	BOOLEAN NoPushFlag = false;
	std::string searchString;
	std::string displayString = "display";
	FileSearch &fs_;
	std::string store_ = "";
};


#endif