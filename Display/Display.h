#ifndef DISPLAY_H
#define DISPLAY_H

///////////////////////////////////////////////////////////////////
//  Display.h      -  header file for Display class              //
//  ver 1.0                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
// Modified By:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

/*
Package Operations:
=================
This package uses zApplicationHelpers package to display customized message


Public Interface:
=================
void static displayConnectionToConsole(std::string string, int port)  - display connection estalished to console
void static displayToConsole(std::string type, std::string list, char seperator) - display message to console
void static displayString(std::string string, int bytes, int port) - display file status while sending to console
void static displayString(std::string string) - display message to console

*/
/*  Required Files :
*-------------- -
*
*Display.h,Display.cpp
*
*  Build Command :
*--------------
*  devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
* ver 1.1 - 28 Apr 16
* - second release
*  ver 1.0 : 09 Apr 16
* -first release
*/

#include<string>
#include<iostream>
#include<vector>
#include<sstream>
class Display
{
public:
	void static displayString(std::string string);
	void static displayConnectionToConsole(std::string string, int port);
	void static displayString(std::string type, std::string string, int bytes, int port, std::string fileAddress="");
	void static displayToConsole(std::string list, char seperator);
};


#endif
