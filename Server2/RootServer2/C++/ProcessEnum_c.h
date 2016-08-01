#ifndef PROCESS_ENUM_C_H
#define PROCESS_ENUM_C_H
/////////////////////////////////////////////////////////////////////
//  ProcessEnum_c.h - Enumerates the processes in a Windows system //
//  ver 1.0                                                        //
//                                                                 //
//  Lanaguage:    Visual C++, Visual Studio 2008                   //
//  Platform:     WinXP                                            //
//  Application:  Help For Project 3, Spring 2009                  //
//  Author:       Phil Pratt-Szeliga, CST 4-116,                   //
//                Syracuse University, (315) 235-5584,             //
//                phil.pratt.szeliga@gmail.com                     //
/////////////////////////////////////////////////////////////////////
/*
    Module Operations:
    ==================
	This module provides a static function that gets the processes
	in a windows system.  It returns a vector<pair<string, int>>.
	Each pair contains the process exe name (the string) and the process 
	id (the int).  This example is based on the website:
	http://www.codeproject.com/KB/threads/Tasks.aspx

    Public Interface:
    =================
	std::vector<std::pair<std::string, int>> processes;   //Make a vector to hold the processes
	processes = ProcessEnum_c::GetProcesses();            //Get the processes
	//use the vector here

*/
/////////////////////////////////////////////////////////////////////
//  Build Process                                                  //
/////////////////////////////////////////////////////////////////////
//  Required Files:                                                //
//    ProcessEnum_c.cpp, ProcessEnum_c.h                           //
//                                                                 //
//  Compiler Command:                                              //
//    cl /EHsc /DTEST_PROCESS_ENUM_C ProcessEnum_c.cpp             //
/////////////////////////////////////////////////////////////////////
/*
    Maintenance History:
    ====================
    ver 1.0 : 30 March 2009
      - first release
*/

#include <windows.h>
#include <tlhelp32.h>		//for MODULEENTRY32
#include <vector>
#include <string>
#include <iostream>

class ProcessEnum_c 
{
public:
	//Get the current running processes in the system
	static std::vector<std::pair<std::string, int>> GetProcesses();

private:
	static char wide_to_narrow(WCHAR c);
};

#endif //PROCESS_ENUM_C_H