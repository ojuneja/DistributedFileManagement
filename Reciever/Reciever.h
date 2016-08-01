#ifndef RECIEVER_H
#define RECIEVER_H

///////////////////////////////////////////////////////////////////
//  Receiver.h      -  header file for Receiver class            //
//  ver 1.4                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
//  Author:       Jim Fawcett, CSE#687 - OOD, Spring 2015        //
//                CST 4-187, Syracuse University, 315 443-3948,  //
//                jfawcett@twcny.rr.com                          //
// Modified By:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

/*
Package Operations:
=================
This package is responsible for receiving byte buffers and strings from sender.This package uses sockets to receive messages.



Public Interface:
=================
void operator()(Socket& socket_)                                            - callable object to receive string or byte buffer 
bool uploadingFile(Socket& socket, std::map<std::string, std::string> map)  - write buffer into specified file
void handleFile(Socket& socket, std::map<std::string, std::string> map)     - handles file by using file upload fucntion to write buffer into file
void fileAndTextSearching(std::map<std::string, std::string> map)          - process file and text searching if a command related to searching comes
void ackFileSearch(Message m, std::map<std::string, std::string> map)      - process file serch acknowledgement if suitable command comes
void performOperations(Socket &socket_, Message m,std::string command, std::string mapCommand, std::map<std::string, std::string> map) - performs operations according to HTTP Message Header
bool uploadingFileOperation(Socket& socket,std::string string_, std::string command, int content, std::string fileName_, int port, std::string category) - upload file
std::string setPath(std::string command_, std::string category, std::string fileName_) - set path where uploading file should be done
*/
/*  Required Files :
*-------------- -
*
* Receiver.h,Receiver.cpp,Message.h,Message.cpp,Sockets.h,Sockets.cpp,Display.h,Display.cpp,FileManager.h,Filemanager.cpp,FileSearch.h,FileSearch.cpp,FileSystem.h,FileSystem.cpp
*
*  Build Command :
*--------------
*  devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
*  ver 1.4 : 26 Apr 2015 - added some functions
*  ver 1.3 : 09 Apr 15
* - commenting
*  ver 1.2 : 05 Apr 15
*  - done major changes by implementing logic of writing buffers to files and modified logic of callable object
*  ver 1.1 : 30 Mar 15
*  - minor modification to comments, above
*  ver 1.0 : 30 Mar 15
*  - first release
*/
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include <iostream>
#include <thread>
#include"../Message/Message.h"
#include<stdlib.h>
#include<fstream>
#include"../QServer/Cpp11-BlockingQueue.h"
#include"../QServer/QServer.h"
#include"../FileManager/FileManager.h"
#include"../Display/Display.h"

using namespace ApplicationHelpers;

class Reciever
{
public:
	Reciever(BlockingQueue<std::string> &queue):queue_(queue){}
	void operator()(Socket& socket_);
	void handleFile(Socket& socket, std::map<std::string, std::string> map);
	bool uploadingFile(Socket& socket, std::map<std::string, std::string> map);
	std::string search(std::string DirectoryInformation, BOOLEAN flag = false);
	void sendFile(Socket& si, std::string file, int sourcePort, int destinationPort);
	void fileAndTextSearching(std::map<std::string, std::string> map);
	void ackFileSearch(Message m, std::map<std::string, std::string> map);
	void performOperations(Socket &socket_, Message m,std::string command, std::string mapCommand, std::map<std::string, std::string> map);
	bool uploadingFileOperation(Socket& socket,std::string string_, std::string command, int content, std::string fileName_, int port, std::string category);
	std::string setPath(std::string command_, std::string category, std::string fileName_);
private:
	BlockingQueue<std::string> &queue_;
};

#endif