#ifndef SERVER1_H
#define SERVER1_H



///////////////////////////////////////////////////////////////////
//  Server1.h      -  header file for Server1 class              //
//  ver 1.2                                                      //
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
This package is responsible for sarting sender and receiver threads.
receiver threads listens at specified port and sender thread connects to specified port.
uses sender's sendfile and sendstring functinality and tells sender what to do by sending commands which are enqueued



Public Interface:
=================
void startReceiever(int port)            - receiver threads starts listeneing at specified port
void startSender(int port)               - connects to server of specified port and tells sender what to send by using commands
void startSenderThread(int port)         - starts Receiver thread
void startReceieverThread(int port)      - starts Sender thread
bool connectToClient(SocketConnecter &si,std::string ipAddress,int port)  - connect to client at specified port
void connectAndPerformOperation(SocketConnecter &si,Sender cl, std::map<std::string, std::string> map_, int port) - connects to client and perform operation
*/
/*  Required Files :
*-------------- -
*
*Receiver.cpp,Message.h,Message.cpp,Sockets.h,Sockets.cpp,Sender.h,Sender.cpp,Display.h,Display.cpp
* FileManager.h,Filemanager.cpp,FileSearch.h,FileSearch.cpp,FileSystem.h,FileSystem.cpp,Server1.h,server1.cpp
*
*  Build Command :
*--------------
*  devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
*  ver 1.2 : 26 Apr 2016 - modified some functions
*  ver 1.1 : 09 Apr 16
* -minor changes by adding command line arguments
*  ver 1.0 : 05 Apr 16
* -first release
*/
#include<iostream>
#include"../Sender/Sender.h"
#include"../Reciever/Reciever.h"
#include"../QServer/Cpp11-BlockingQueue.h"
#include"../Display/Display.h"

class Server1
{
public:
	Server1() :rec_(receiveingQueue_){
	}
	void startReceiver(int port);
	void startSender(int port);
	void startSenderThread(int port);
	void startReceiverThread(int port);
	bool connectToClient(SocketConnecter &si, std::string ipAddress, int port);
	void connectAndPerformOperation(SocketConnecter &si, Sender cl, std::map<std::string, std::string> map_, int port);
	void startServer(int port);
	int port_;
	BlockingQueue<std::string> receiveingQueue_;
private:
	Reciever rec_;
	std::string address_;
};

#endif