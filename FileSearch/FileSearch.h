#ifndef FILESEARCH_H
#define FILESEARCH_H


///////////////////////////////////////////////////////////////////
//  FileSearch.h      -  header file for Filesearch class        //
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
This package performs operations like searching of text in particular files.
This package is capable of searching text or line in particular files as specified
by path.
While searching again, this package takes files and path from map stored in datastore
and search text in those files.
This package also process the inputs(/f<searchtext> <pattern/patterns>) which is 
entered second time in console.


Public Interface:
=================

void findText(Store store_, std::string)                        - takes display info and map(where files and path iterators are stored) as arguments 
                                                                  and generates the absolute path and controls he operations of other functions 
																  like fileSearch and filterFiles.
BOOLEAN fileSearch(std::string, std::string)                    - searches text in files and returns true if a particular text is found in file. 
std::string filterFiles(std::vector<std::string> strPatternsandText_,std::string file)   - filter the files according to pattern mentioned.


*/
/*Required Files :
*---------------- -
*FileSystem.h , FileSystem.cpp , FileSearch.h , FileSearch.cpp
*
*  Build Command :
*----------------
*    devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*----------------------
* ver 2.1 : 26 Apr 2015 - removed some functions
*Ver 2.0 : 9 Feb 15
* -second release
*Ver 1.0 : 6 Feb 15
* -first release
*/

#include<iostream>
#include<vector>
#include<map>
#include <string>
#include<set>
#include<list>
#include<sstream>
#include"..//FileSystem/FileSystem.h"
#include<regex>
class FileSearch
{
public:
	using vector = std::vector < std::string >;
	using File = std::string;
	std::string findText(std::string filePaths, std::string searchString);
	BOOLEAN fileSearch(std::string, std::string);
	std::string filterFiles(std::vector<std::string> strPatternsandText_,std::string file);
	std::vector<std::string> FileSearch::split(std::string filePaths, char separator);
private:
	std::string searchString;
	std::vector<std::string> patternsVector;
	BOOLEAN booleanFindFlag = false;
};


#endif