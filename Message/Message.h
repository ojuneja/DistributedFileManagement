#ifndef MESSAGE_H
#define MESSAGE_H
///////////////////////////////////////////////////////////////////
//  Message.h      -  header file for Message class              //
//  ver 1.2                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
// Author:    Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

/*
Package Operations:
=================
This package contains a structure header and class that build HTTP header which contains information.
This information tells client/server about what actions to be performed.


Public Interface:
=================
std::vector<std::string> splitting(std::string message, char seperator) -  split message by using delimiter specified by user
std::string makeHeader()                                                -  constructs HTTP Message
std::map<std::string, std::string> makeMap(std::vector<std::string> vec)-  maps the header title with its value
std::string makeQueueCommand(std::string comm, std::string fileName, std::string option, std::string destinationAddress, int port, std::string text = "") - constrcuts queue commands
*/
/*  Required Files :
*-------------- -
*
* Message.h Message.cpp,Sockets.h Sockets.cpp
*
*  Build Command :
*--------------
*  devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
*  ver 1.2 : 26 Apr 2016 - done minor changes related to header
*  ver 1.1 : 09 Apr 16
* -minor changes to HTTP Header
*  ver 1.0 : 05 Apr 16
* -first release
*/
#include<String>
#include<vector>
#include<sstream>
#include<iostream>
#include<map>
#include"../Sockets/Sockets.h"

//declaration of header variables
struct Header
{
	std::string command_;
	std::string SourceIpAddress_;
	std::string DestinationIpAddress_;
	std::string MessageType_;
	int bodycontentlength_;
	std::string filename_;
	int sourcePort_;
	int destinationPort_;
	std::string category_;
};



class Message
{
public:
	Message() {}
	//initializing the header variables through connstructor
	Message(std::string comm, std::string SourceIp, std::string DestinationIp,  int bodylength,
		int sourcePort, int destinationPort, std::string file = "", std::string category = "")
	{
		h->command_ = comm;
		h->SourceIpAddress_ = SourceIp;
		h->DestinationIpAddress_ = DestinationIp;
		h->bodycontentlength_ = bodylength;
		h->filename_ = file;
		h->sourcePort_ = sourcePort;
		h->destinationPort_ = destinationPort;
		h->category_ = category;
	}
	std::vector<std::string> splitting(std::string message, char seperator);
	std::string makeHeader();
	std::map<std::string, std::string> makeMap(std::vector<std::string> vec);
	std::string makeQueueCommand(std::string comm, std::string fileName, std::string option, std::string destinationAddress, int port, std::string text = "");
private:
	Header *h = new Header();
	std::string message;
};






#endif