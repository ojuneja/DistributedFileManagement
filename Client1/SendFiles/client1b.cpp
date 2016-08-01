///////////////////////////////////////////////////////////////////
//  Sender.cpp      -  implementation file for Sender class      //
//  ver 1.3                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Message,Project - 3(Message Passing Communication)//
//  Author:       Jim Fawcett, CSE#687 - OOD, Spring 2015        //
//                CST 4-187, Syracuse University, 315 443-3948,  //
//                jfawcett@twcny.rr.com                          //
// Modified By:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////



#include"Sender.h"

//send byte buffers to receiver
void Sender::sendFile(Socket& si, std::string category,std::string file, int sourcePort, int destinationPort,BOOLEAN flag)
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
	Display::displayString("Uploading", file, size, destinationPort, fileAddress);//displaying if the file starts uploading
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
			if (flag == true)
			{
				Message m("1", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
				header = m.makeHeader();
			}
			else
			{
				Message m("11", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
				header = m.makeHeader();
			}
		}
		else
		{
			if (flag == true)
			{
				Message m("0", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
				header = m.makeHeader();
			}
			else
			{
				Message m("10", "localhost", "localhost", bSize, sourcePort, destinationPort, file, category);
				header = m.makeHeader();
			}
		}
		si.sendString(header);
		infile.read(buffer, bSize); //reading the file and copy contents int buffer
		si.send(bSize,buffer);
		if (infile.eof())
			break;
	}
	infile.close();
}


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
		Display::displayString("\nPacking message from SenderSide,Message is : \n" + header);
	}
	else if (message == "DOWNLOAD")
	{
		Message m("8", "localhost", "localhost", 0, sourcePort, destinationPort, files, serverPath);
		header = m.makeHeader();
		Display::displayString("\nPacking message from SenderSide,Message is : \n" + header);
	}
  si.sendString(header);
}

void Sender::sendStringBuffer(Socket& si, std::string string, int sourcePort, int destinationPort)
{
	const int bufferLength = 20;//buffer size
	int size_ = string.size();
	Socket::byte buffer[bufferLength];
	std::string header;
	int counter= 0;
	//Display::displayString("Sending", string, size_, destinationPort);//displaying if the file starts uploading
	while (size_>0)
	{
		std::memset(buffer, 0, 20);
		int bSize = 19;
		if (size_ < bSize) {
			bSize = size_;
		}
		size_ = size_ - bSize;
		for (int i = 0; i < bSize; i++)
		{
			buffer[i] = string[counter];
			counter++;
		}
		if (size_ == 0)//if this is the last byte buffers from string then sending the buffers with header as command 3 indicating that this is string end
		{
			Message m("3", "localhost", "localhost", bSize, sourcePort, destinationPort, "");
			header = m.makeHeader();
			buffer[bSize] = '\0';
		}
		else
		{
			Message m("2", "localhost", "localhost", bSize, sourcePort, destinationPort, "");
			header = m.makeHeader();
			buffer[bSize] = '\0';
		}
		
		si.sendString(header);
		si.send(bSize+1, buffer);
	}
}



void Sender::sendDirectoryPath(Socket& si, std::string directoryPath, std::string pattern, int sourcePort, int destinationPort,std::string text)
{
	std::string directoryInformation = "";
	if (pattern == "")
	{
		pattern = "*.*";
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
    Verbose v(true);
    SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 9080))
    {
      Verbose::show("client waiting to connect");
      ::Sleep(100);
    }
	cl.sendFile(si, "demo.txt","", 9089, 9080);
	cl.sendStringBuffer(si, "abc", 9089, 9080);
	cl.sendStringSender(si, 9089, 9080,"DONE","demo.txt");
    Verbose::show("\n  client calling send shutdown\n");
    si.shutDownSend();
  }
  catch (std::exception& ex)
  {
    Verbose::show("  Exception caught:", always);
    Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
  }
}
#endif
