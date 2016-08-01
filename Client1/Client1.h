#ifndef CLIENT1_H
#define CLIENT1_H

///////////////////////////////////////////////////////////////////
//  Client1.h      -  header file for Client1 class              //
//  ver 1.1                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
// Author:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

/*
Package Operations:
=================
This package is responsible for sarting sender and receiver threads.
receiver threads listens at specified port and sender thread connects to specified port.
uses sender's sendfile and sendstring functinality and tells sender what to do by sedning commands which are enqueued



Public Interface:
=================
void startReceiever(int port)            - receiver threads starts listeneing at specified port
void startSender(int port)               - connects to server of specified port and tells sender what to send by using commands
void startSenderThread(int port)         - starts Receiver thread
void startReceieverThread(int port)      - starts Sender thread
bool connectToServer(SocketConnecter &si,std::string ipAddress,int port)  - connect to server at specified port
void postMessage(std::string msg)        - enqueue client queue
std::string getMessage()                 - dequeue client queue
void connectAndPerformOperation(SocketConnecter &si,Sender cl, std::map<std::string, std::string> map_, int port) - connects to server and perform operation
*/
/*  Required Files :
*-------------- -
*
* Client1.h,Client1.cpp,Receiver.h,Receiver.cpp,Message.h,Message.cpp,Sockets.h,Sockets.cpp,Sender.h,Sender.cpp,Display.h,Display.cpp
*
*  Build Command :
*--------------
*  devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
* ver 1.2 : 28 Apr 16 - added some functions
*  ver 1.1 : 09 Apr 16
* -minor changes by adding command line arguments
*  ver 1.0 : 05 Apr 16
* -first release
*/

#include<iostream>
#include<array>
#include"../Sender/Sender.h"
#include"../Reciever/Reciever.h"
#include"../QServer/QServer.h"
#include"../Display/Display.h"

class Client1
{
public:
	
	Client1() :rec_(receiveingQueue_){
	}
	void startReceiever(int port);
	void startSender(int port);
	void startSenderThread(int port);
	void startReceieverThread(int port);
	void postMessage(std::string msg);
	std::string getMessage();
	void startClient(int port);
	bool connectToServer(SocketConnecter &si, std::string ipAddress, int port);
	void connectAndPerformOperation(SocketConnecter &si,Sender cl, std::map<std::string, std::string> map_, int port);
	BlockingQueue<std::string> sendingQueue_;
	BlockingQueue<std::string> receiveingQueue_;
	int port_;
private:
	std::string address_="localhost";
	Reciever rec_;
	
};

#endif