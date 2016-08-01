#ifndef CHANNEL_H
#define CHANNEL_H
///////////////////////////////////////////////////////////////////
//  Channel.h      -  header file for Channel class              //
//  ver 1.2                                                      //
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
This package acts as a channel for communication between managed and unmanaged code
That ensures that the managed code is isolated from all the implementation details of the native code.
Channel package shows how to post and get messages from native code to the Managed UI.



Public Interface:
=================

virtual void postMessage(const std::string& command, const std::string& msg, const std::string &category, const std::string &searchText, int port) = 0 - when implemented responsible for calling post message of client
virtual std::string getMessage() = 0 - when implemented responsible for calling get message of client
*/
/*  Required Files :
*-------------- -
*
* Client1.h,Client1.cpp,Receiver.h,Receiver.cpp,Message.h,Message.cpp,Sockets.h,Sockets.cpp,Sender.h,Sender.cpp,Display.h,Display.cpp
* FileManager.h,Filemanager.cpp,FileSearch.h,FileSearch.cpp,FileSystem.h,FileSystem.cpp
*
*  Build Command :
*--------------
* devenv CppCli-WPF-App.sln / rebuild debug
*
*  Maintenance History :
*--------------------
*  ver 1.2 : 26 Apr 2016
*  ver 1.1 : 09 Apr 16
* -minor changes by adding command line arguments
*  ver 1.0 : 05 Apr 16
* -first release
*/


#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>

struct IChannel
{
public:
	virtual void start() = 0;
};

struct IClient
{
public:
	virtual void postMessage(const std::string& command, const std::string& msg, const std::string &category, const std::string &searchText, int port) = 0;
	virtual std::string getMessage() = 0;
};

//Object factory ensures that the managed code is isolated from all the implementation details of the native code. 
extern "C" {
	struct ObjectFactory
	{
		DLL_DECL IClient* createClient(int port);
		DLL_DECL IChannel* createChannel(IClient* client);
		
	};
}

#endif


