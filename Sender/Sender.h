#ifndef SENDER_H
#define SENDER_H
///////////////////////////////////////////////////////////////////
//  Sender.h      -  header file for Sender class                //
//  ver 1.4                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
//  Author:       Jim Fawcett, CSE#687 - OOD, Spring 2016        //
//                CST 4-187, Syracuse University, 315 443-3948,  //
//                jfawcett@twcny.rr.com                          //
// Modified By:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

/*
Package Operations:
=================
This package is responsible for sending byte buffers and strings to reciever.This package uses sockets to send messages.



Public Interface:
=================
void sendFile(Socket& si, std::string file, int sourcePort=0, int destinationPort=0) - send byte buffers to receiver
void sendStringSender(Socket& si, int  sourcePort, int destinationPort, std::string message,std::string fileName="") - send strings to receiver
void sendDirectoryPath(Socket& si, std::string directoryPath, std::string pattern, int sourcePort, int destinationPort,std::string text="") -  send directory,path,pattern info to receiver
std::string  makeHeader(bool flag, std::string header, int bSize, int sourcePort, int destinationPort, std::string file, std::string category) -  make header
std::string makeHeaderEnd(bool flag, std::string header, int bSize, int sourcePort, int destinationPort, std::string file, std::string category)- make header
*/
/*  Required Files :
*-------------- -
*
* Sender.h,Sender.cpp,Sockets.h,Sockets.cpp,Message.h,Message.cpp,Display.h,Display.cpp,FileManager.h,Filemanager.cpp,FileSearch.h,FileSearch.cpp,FileSystem.h,FileSystem.cpp
*
*  Build Command :
*--------------
*  devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
*  ver 1.4 : 26 Apr 2016 - added some functions
*  ver 1.3 : 09 Apr 16
* - commenting
*  ver 1.2 : 05 Apr 16
*  - done major changes by implementing logic of sending buffers so that binary files can be sent and modified logic of send string
*  ver 1.1 : 30 Mar 16
*  - minor modification to comments, above
*  ver 1.0 : 30 Mar 16
*  - first release
*/
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include"../Message/Message.h"
#include <iostream>
#include<fstream>
#include"../Display/Display.h"
#include"../FileSystem/FileSystem.h"
using namespace ApplicationHelpers;

class Sender
{
public:
	void sendFile(Socket& si, std::string category, std::string file, int sourcePort = 0, int destinationPort = 0,bool flag = true);
	void sendStringSender(Socket& si, int  sourcePort, int destinationPort, std::string message, std::string fileName = "",std::string serverPath="");
	void sendDirectoryPath(Socket& si, std::string directoryPath, std::string pattern, int sourcePort, int destinationPort,std::string text="");
	std::string  makeHeader(bool flag, std::string header, int bSize, int sourcePort, int destinationPort, std::string file, std::string category);
	std::string makeHeaderEnd(bool flag, std::string header, int bSize, int sourcePort, int destinationPort, std::string file, std::string category);
};


#endif