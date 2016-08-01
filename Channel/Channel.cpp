///////////////////////////////////////////////////////////////////
//  Channel.cpp      -  implementation file for Channel class    //
//  ver 1.2                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
// Author:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////


#define IN_DLL
#include "Channel.h"
#include "../QServer/QServer.h"
#include "..//Client1/Client1.h"
#include <string>
#include <iostream>

class Channel :public IChannel
{
public:
	Channel(IClient *cl1):cl1_(cl1){};
	void start();
private:
	IClient *cl1_;
};

class Client :public IClient
{
public:
	//constructor initiates isntanse of client1 with the specified port
	Client(int port) :port_(port){ client1 = new Client1(); client1->startClient(port_); }
	void postMessage(const std::string& command, const std::string& msg, const std::string &category, const std::string &searchText, int port);
	std::string getMessage();
private:
	Client1 *client1;
	int port_;
};

//Responsible for calling post message of client and queues command into client queue
void Client::postMessage(const std::string& command, const std::string& msg, const std::string &category, const std::string &searchText, int port)
{
	Message m;
	std::string command_ = "";
	//Peforms operation according to following commands
	if (command == "FINDFILES" || command == "UPLOAD" || command == "DOWNLOAD")
	{
		command_ = m.makeQueueCommand(command, category,msg, "localhost", port);
	}
	else if (command == "SEARCHTEXT")
	{
		command_ = m.makeQueueCommand(command, category, msg, "localhost", port, searchText);
	}
	client1->postMessage(command_);
}

//Responsible for calling get message of client
std::string Client::getMessage()
{
	return client1->getMessage();
}



void Channel::start()
{
	Display::displayString("Demonstrating Requirement 4 by using GUI \n Channel started");
	Display::displayString("Client1 has file repository on path : .\\Client1\\SendFiles(you can upload files from this repository)");
	Display::displayString("Downloaded files will be received in .\\Client1\\ReceivedFiles");
}



IChannel* ObjectFactory::createChannel(IClient *client)
{
	return new Channel(client);
}

IClient* ObjectFactory::createClient(int port)
{
	return new Client(port);
}

#ifdef TEST_CHANNEL
int main()
{
	Client *channel_;
	std::cout << "\n Starting client1 through our channel \n";
	channel_ = new Client(8089);
	return 0;
}
#endif