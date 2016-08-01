///////////////////////////////////////////////////////////////////
//  Sender.cpp      -  implementation file for Sender class      //
//  ver 1.4                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Project - 4(Remote Code Management)              //
//                jfawcett@twcny.rr.com                          //
//Author:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////



#include"Sender.h"

//send byte buffers to receiver
void Sender::sendFile(Socket& si, std::string category,std::string file, int sourcePort, int destinationPort,bool flag)
{
	std::ifstream infile;
	int size = 0;
	char buffer[1024];//buffer size
	std::string header = "";
	std::string fileAddress = file;
	infile.open(fileAddress, std::ios::binary | std::ios::ate | std::ios::in);//opens files in binary mode
	if (infile.is_open())
	{
		size = static_cast<int>(infile.tellg()); //find the size of file
	}
	infile.seekg(0, infile.beg);
	while (size > 0)
	{
		std::memset(buffer, 0, 1024);//clearing the buffer
		int bSize = 1024;
		if (size < bSize) {
			bSize = size;
		}
		size = size - bSize;
		if (size == 0) //if this is the last byte buffers from file then sending the buffers with header as command 1 indicating that this is file end
		{
			header = makeHeaderEnd(flag, header, bSize, sourcePort, destinationPort,file,category);
		}
		else
		{
			header = makeHeader(flag, header, bSize, sourcePort, destinationPort, file, category);
		}
		si.sendString(header);
		infile.read(buffer, bSize); //reading the file and copy contents int buffer
		si.send(bSize,buffer);
		if (infile.eof())
			break;
	}
	infile.close();
}

//makes header which is to be sent to receiver
std::string Sender::makeHeaderEnd(bool flag, std::string header, int bSize, int sourcePort,int destinationPort,std::string file,std::string category)
{
	if (flag == true)
	{
		//sets command to 1 if it is the end of file in case of uploading
		Display::displayString("Uploading File " + file + " to server at port: " + std::to_string(destinationPort));//displaying if the file starts uploading
		Message m("1", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
		header = m.makeHeader();
	}
	else
	{
		//sets command to 11 if it is the end of file in case of downloading
		Display::displayString("Sending File " + file + " from server to Client at port: " + std::to_string(sourcePort));//displaying if the file starts downloading
		Message m("11", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
		header = m.makeHeader();
	}
	return header;
}

//makes header which is to be sent to receiver
std::string Sender::makeHeader(bool flag, std::string header, int bSize, int sourcePort, int destinationPort, std::string file, std::string category)
{
	if (flag == true)
	{
		//sets command to 0 in case of uploading
		
		Message m("0", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
		header = m.makeHeader();
	}
	else
	{
		//sets command to 10 in case of downloading
		
		Message m("10", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
		header = m.makeHeader();
	}
	return header;
}

//send strings to receiver
void Sender::sendStringSender(Socket& si, int sourcePort, int destinationPort, std::string message, std::string files,std::string serverPath)
{
	std::string header = "";
	if (message == "DONEUPLOAD")
	{
		Message m("4", "localhost", "localhost", 0, sourcePort, destinationPort, files, serverPath);
		header = m.makeHeader();
	}
	if (message == "DONEDOWNLOAD")
	{
		Message m("5", "localhost", "localhost", 0, sourcePort, destinationPort, files, serverPath);
		header = m.makeHeader();
	}
	else if (message == "CLOSE")
	{
		Message m("99", "localhost", "localhost", 0, sourcePort, destinationPort);
		header = m.makeHeader();
	}
	else if (message == "LIST")
	{
		Message m("7", "localhost", "localhost", 0, sourcePort, destinationPort, files);
		header = m.makeHeader();
	}
	else if (message == "DOWNLOAD")
	{
		Message m("8", "localhost", "localhost", 0, sourcePort, destinationPort, files, serverPath);
		header = m.makeHeader();
	}
  si.sendString(header);
}


//send directory,path,pattern info to receiver
void Sender::sendDirectoryPath(Socket& si, std::string directoryPath, std::string pattern, int sourcePort, int destinationPort,std::string text)
{
	std::string directoryInformation = "";
	if (pattern == "")
	{
		pattern = "*.*"; //sets pattern to *.* if nothing is encountered
	}
	if (text == "")
	{
		
		directoryInformation = directoryPath + "|" + pattern;
		Message m("6", "localhost", "localhost", 0, sourcePort, destinationPort, directoryInformation);
		si.sendString(m.makeHeader());
	}
	else
	{
		directoryInformation = directoryPath + "|" + pattern + "|" + text;
		Message m("9", "localhost", "localhost", 0, sourcePort, destinationPort, directoryInformation);
		si.sendString(m.makeHeader());
	}
	
}

//----< demonstration >------------------------------------------------------
#ifdef TEST_SENDER
int main()
{
  title("Testing Socket Client", '=');
  Sender cl;
  try
  {
    SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 9080))
    {
      ::Sleep(100);
    }
	std::cout << "\n"<<"Sender sending File: demo.txt to Receiver listening at port:9080"<<"\n";
	cl.sendFile(si, "..\\Reciever\\RecievedFiles", "demo.txt", 9089, 9080);
    si.shutDownSend();
  }
  catch (std::exception& ex)
  {
	  std::cout << ex.what();
   /* Verbose::show("  Exception caught:", always);
    Verbose::show(std::string("\n  ") + ex.what() + "\n\n");*/
  }
}
#endif
