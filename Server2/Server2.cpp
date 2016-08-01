///////////////////////////////////////////////////////////////////
//  Server2.cpp      -  implementation file for Server2 class    //
//  ver 1.2                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
// Author     :   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

#include"Server2.h"

//receiver threads starts listeneing at specified port
void Server2::startReceiver(int port)
{
	try
	{
		Display::displayConnectionToConsole("Server2 Receiver", port);
		Display::displayString(std::string(60, '='));
		Display::displayString("All Files will be Received inside subdirectories of .\\Server2\\RootServer2\\");
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

//connects to server of specified port and tells sender what to send by using commands
void Server2::startSender(int port)
{
	Sender cl;
	try
	{
		Message msg;
		SocketSystem ss;
		SocketConnecter si;
		while (true)
		{
			//dequeuing the messages from queue one by one
			std::vector<std::string> vec = msg.splitting(receiveingQueue_.deQ(), '\n');
			std::map<std::string, std::string> map_ = msg.makeMap(vec);//storing the command in a map to compare key and value later
			//identifying the command and performing particular operations
			connectAndPerformOperation(si, cl, map_, port);
		}
	}
	catch (std::exception& ex)
	{
		Display::displayString("  Exception caught:");
		Display::displayString(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//connects to client and perform operation
void Server2::connectAndPerformOperation(SocketConnecter &si, Sender cl, std::map<std::string, std::string> map_, int port)
{
	if (map_["Command"] == "DONEUPLOAD")
	{
		if (connectToClient(si, map_["DestinationIpAddress"], stoi(map_["DestinationPort"])))//connecting to client each time it gets command
		{
			cl.sendStringSender(si, stoi(map_["SourcePort"]), stoi(map_["DestinationPort"]), map_["Command"], map_["FileName"], map_["Category"]);
		}
	}
	else if (map_["Command"] == "LIST")
	{
		if (connectToClient(si, map_["DestinationIpAddress"], stoi(map_["DestinationPort"])))//connecting to client each time it gets command
		{
			cl.sendStringSender(si, stoi(map_["SourcePort"]), stoi(map_["DestinationPort"]), map_["Command"], map_["FileName"]);
		}
	}
	else if (map_["Command"] == "DOWNLOAD")
	{
		if (connectToClient(si, map_["DestinationIpAddress"], stoi(map_["DestinationPort"])))//connecting to client each time it gets command
		{
			cl.sendFile(si, map_["Category"], map_["FileName"], stoi(map_["SourcePort"]), stoi(map_["DestinationPort"]), false);
		}
	}
	else if (map_["Command"] == "DONEDOWNLOAD")
	{
		if (connectToClient(si, map_["DestinationIpAddress"], stoi(map_["DestinationPort"])))//connecting to client each time it gets command
		{
			cl.sendStringSender(si, stoi(map_["SourcePort"]), stoi(map_["DestinationPort"]), map_["Command"], map_["FileName"], map_["Category"]);
		}
	}
}


//connect to client at specified port
bool Server2::connectToClient(SocketConnecter &si, std::string ipAddress, int port)
{
	while (!si.connect(ipAddress, port))
	{
		Display::displayString("client waiting to connect");
		::Sleep(100);
	}
	return true;
}

//starts server thread
void Server2::startServer(int port)
{
	startReceieverThread(port);//starting receiver thread
	startSenderThread(port);//starting sender thread
}

//starts Sender thread
void Server2::startSenderThread(int port)
{
	std::thread clientThread(&Server2::startSender, this,port);
	clientThread.join();
}

//starts Receiver thread
void Server2::startReceieverThread(int port)
{
	std::thread serverThread(&Server2::startReceiver, this, port);
	serverThread.detach();
}



#ifdef TEST_SERVER2
int main(int argc,char *argv[])
{
	Server2 sr2;
	if (argc > 1)
	{
		sr2.port_ = atoi(argv[1]);
	}
	else
		sr2.port_ = 9000;
	sr2.startServer(sr2.port_);
	return 0;
}
#endif