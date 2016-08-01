///////////////////////////////////////////////////////////////////
//  Client1.cpp      -  implementation file for Client1 class    //
//  ver 1.2                                                     //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
// Author:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

#include"Client1.h"

//receiver threads starts listeneing at specified port
void Client1::startReceiever(int port)
{
	try
	{
		Display::displayConnectionToConsole("Client1 Receiver",port);
		Display::displayString(std::string(60, '='));
		SocketSystem ss;
		SocketListener sl(port, Socket::IP6);
		sl.start(rec_);	
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Display::displayString("  Exception caught:");
		Display::displayString(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//enqueue message into client's queue
void Client1::postMessage(std::string msg)
{
	sendingQueue_.enQ(msg);
}

//dequeue message from client's queue
std::string Client1::getMessage()
{

	std::string result = receiveingQueue_.deQ();
	return result;
}

//starts the client threads
void Client1::startClient(int port)
{
	startReceieverThread(port);
	startSenderThread(port);
}

//connects to server of specified port and tells sender what to send by using commands
void Client1::startSender(int port)
{
	try
	{
		Sender cl;
		Message msg;
		SocketSystem ss;
		SocketConnecter si;
		while (true){
			auto item = sendingQueue_.deQ();//dequeuing the commands from queue one by one
			std::vector<std::string> vec = msg.splitting(item, '$');
			std::map<std::string, std::string> map_ = msg.makeMap(vec);//storing the command in a map to compare key and value later
			connectAndPerformOperation(si,cl,map_,port);
		}
	}
	catch (std::exception& ex)
	{
		Display::displayString("  Exception caught:" + std::string("\n  ") + ex.what() + "\n\n");
	}
}

void Client1::connectAndPerformOperation(SocketConnecter &si,Sender cl, std::map<std::string, std::string> map_, int port)
{
	if (map_["Command"] == "UPLOAD" && map_["FileName"].size() > 0)//identifying the commands and then performing particular operations
	{
		if (connectToServer(si, map_["DestinationAddress"], stoi(map_["DestinationPort"])))//connecting to server each time it gets command
		{
			cl.sendFile(si, map_["FileName"], map_["Option"], port, stoi(map_["DestinationPort"]));
		}
	}
	else if (map_["Command"] == "FINDFILES")
	{
		if (connectToServer(si, map_["DestinationAddress"], stoi(map_["DestinationPort"])))
		{
			cl.sendDirectoryPath(si, map_["FileName"], map_["Option"], port, stoi(map_["DestinationPort"]));
		}
	}
	else if (map_["Command"] == "SEARCHTEXT")
	{
		if (connectToServer(si, map_["DestinationAddress"], stoi(map_["DestinationPort"])))
		{
			cl.sendDirectoryPath(si, map_["FileName"], map_["Option"], port, stoi(map_["DestinationPort"]), map_["Text"]);
		}
	}
	else if (map_["Command"] == "DOWNLOAD")
	{
		if (connectToServer(si, map_["DestinationAddress"], stoi(map_["DestinationPort"])))
		{
			cl.sendStringSender(si, port, stoi(map_["DestinationPort"]), map_["Command"], map_["FileName"], map_["Option"]);
		}
	}
	else
	{
		if (connectToServer(si, map_["DestinationAddress"], stoi(map_["DestinationPort"])))
			cl.sendStringSender(si, port_, stoi(map_["DestinationPort"]), map_["Command"], map_["FileName"]);
	}
}

//connect to server at specified port
bool Client1::connectToServer(SocketConnecter &si, std::string ipAddress, int port)
{
	while (!si.connect(ipAddress, port))
	{
		Display::displayString("client waiting to connect");
		::Sleep(100);
	}
	return true;
}

//starts Sender thread
void Client1::startSenderThread(int port)
{
	std::thread senderThread(&Client1::startSender, this, port);
	senderThread.detach();
}

//starts Receiver thread
void Client1::startReceieverThread(int port)
{
	std::thread receiverThread(&Client1::startReceiever,this, port);
	receiverThread.detach();
}


#ifdef TEST_CLIENT1
int main(int argc, char *argv[])
{
	Client1 cl1;
	bool flag = true;
	std::vector<std::string> vec;
	Message msg_;
	if (argc > 1)
		cl1.port_ = atoi(argv[1]);
	else
		cl1.port_ = 8087;
	std::cout << "Client1 started\n\n";
	cl1.startReceieverThread(cl1.port_); //starting receiver thread
	std::string command_ = msg_.makeQueueCommand("UPLOAD", "..\\Client1\\SendFiles\\", "client1a.txt", "localhost", 8087);
	cl1.postMessage(command_);
	cl1.startSenderThread(cl1.port_); //starting receiver thread
	return 0;
}
#endif