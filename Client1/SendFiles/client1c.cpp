#include"Reciever.h"

///////////////////////////////////////////////////////////////////
//  Receiver.cpp      -  implementation file for Receiver class  //
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


void Reciever::operator()(Socket& socket_)
{
	Message m;
	bool flag = false;
  while (true)
  {
    // interpret test command
    std::string command = socket_.recvString();
	//std::cout << command << "\n";
	std::string header = "";
	std::stringstream ss(command);
	std::string item;
	std::vector<std::string> vec = m.splitting(command,'\n');
	//saving the test command in map to use its key value pair
	std::map<std::string, std::string> map = m.makeMap(vec);
	std::string mapCommand = map["Command"];
	std::string fileName = map["FileName"];
	if (command.size() == 0)//if there is no command
	{
		break;
	}
	if ((mapCommand == "0" || mapCommand == "1") && fileName.size() >0)//if the contents from binary/text file is received
	{
		fileName = map["FileName"];
		handleFile(socket_,map);
	}
	else if (mapCommand == "2" || mapCommand == "3")//if the text is received
	{
		handleString(socket_, map);
	}
	else if (mapCommand == "4")//if the file or string is uploaded by client
	{
		if (map["FileName"] == "String")
		{
			Verbose::show("TextString succesfully sent by Client", always);
		}
		else
		{
			Verbose::show("File: '" + map["FileName"] + "' is uploaded", always);
			queue_.enQ("File: " + map["FileName"] + " uploaded at Path: " + map["Category"] + "\\" + map["FileName"]);
		}
	}
	else if (mapCommand == "5")
	{
		queue_.enQ("File: " + map["FileName"] + " downloaded at Path: " + map["Category"] + "\\" + map["FileName"]);
	}
	else if ((mapCommand == "6" || mapCommand == "9") && fileName.length() > 0)
	{
		std::string filePaths = "";
		if (mapCommand == "9")
			{
				filePaths = search(map["FileName"],true);
			}
		else
		{
			filePaths = search(map["FileName"]);
		}
		Message m("LIST", "localhost", "localhost", 0, stoi(map["DestinationPort"]), stoi(map["SourcePort"]), map["DestinationPort"]+ "|" + filePaths);
			queue_.enQ(m.makeHeader());
	}
	else if (mapCommand == "7")
	{

		std::cout << "Files received are: " << "\n";
		std::vector<std::string> vec = m.splitting(map["FileName"],'|');
		for (auto elem : vec)
		{
			std::cout << elem << "\n";
		}
		queue_.enQ(map["FileName"]);
	}
	else if (mapCommand == "8")
	{
		Message m("DOWNLOAD", "localhost", "localhost", 0, stoi(map["SourcePort"]), stoi(map["DestinationPort"]), map["FileName"],map["Category"]);
		queue_.enQ(m.makeHeader());
	}
	else if (mapCommand == "10" || mapCommand == "11")
	{
		fileName = map["FileName"];
		handleFile(socket_, map);
	}
	else//if the command doesnot matches with any provided commands
	{
		break;
	}
  }
}




std::string Reciever::search(std::string DirectoryInformation,BOOLEAN flag)
{
	std::string list;
	std::vector<std::string> fileAndPathVector;
	Message m;
	std::vector<std::string> container = m.splitting(DirectoryInformation, '|');
	FileSearch fs;
	std::string path_ = container[0];
	container.erase(container.begin());
	std::vector<std::string> restOptions = m.splitting(container[0], ' ');
	restOptions.push_back("/s");
	FileManager fm(path_, restOptions, fs);
	list = fm.search();
	if (flag)
	{
		list = fs.findText(list, container[1]);
	}
	return list;
	
}


void Reciever::handleFile(Socket& socket, std::map<std::string, std::string> map)
{
	if (map["FileName"].find_last_of("\\") != std::string::npos)
	{
		int beg = map["FileName"].find_last_of("\\");
		int end = map["FileName"].size();
		std::string fileName_ = map["FileName"].substr(beg + 1, end);
		map["FileName"] = fileName_;
	}
	if (uploadingFile(socket, map))//if the uploading of file is done then enqueue message into queue
	{
		if (map["Command"] == "0")
		{
			Message m("DONEUPLOAD", "localhost", "localhost", 0, stoi(map["DestinationPort"]), stoi(map["SourcePort"]), map["FileName"], map["Category"]);
			queue_.enQ(m.makeHeader());
		}
		else
		{
			Message m("DONEDOWNLOAD", "localhost", "localhost", 0, stoi(map["DestinationPort"]), stoi(map["SourcePort"]), map["FileName"], map["Category"]);
			queue_.enQ(m.makeHeader());
		}

	}
}

void Reciever::handleString(Socket& socket, std::map<std::string, std::string> map)
{
	if (stringReceived(socket, map))//if the full string is received then enqueue message into queue
	{
		Message m("DONEUPLOAD", "localhost", "localhost", 0, stoi(map["DestinationPort"]), stoi(map["SourcePort"]), "String");
		queue_.enQ(m.makeHeader());
	}
}

// write buffer into specified file
bool Reciever::stringReceived(Socket& socket, std::map<std::string, std::string> map)
{
	Message m;
	const int BufLen = 20;//buffer size
	Socket::byte buffer[BufLen];
	bool ok;
	bool flag = false;
	std::string string_;
	int content = stoi(map["BodyContentLength"]);
	std::string command_ = map["Command"];
	int port = stoi(map["SourcePort"]);
	std::string finalString = "";
	while (true)
	{
		if (flag == true) //if the some part of string is yet to be received then get the commands again
		{
			string_ = socket.recvString();
			std::vector<std::string> vec_ = m.splitting(string_, '\n');
			std::map<std::string, std::string> map_ = m.makeMap(vec_);
			command_ = map_["Command"];
			content = stoi(map_["BodyContentLength"]);
			port = stoi(map_["SourcePort"]);
		}
		ok = socket.recv(content+1, buffer); 
		finalString = finalString + buffer;
		if (command_ == "3")//if the buffer from text string is coming for last time
		{
			//Display::displayAcknowledgementToConsole("String",finalString,port);//dispaying if the full string is received
			return true;
		}
		
		if (socket == INVALID_SOCKET || !ok)//if the string is not received properly of socket is invalid then break
			return false;
		flag = true;
	}
	return true;
}

bool Reciever::uploadingFile(Socket& socket, std::map<std::string,std::string> map)
{
	Message m;
	const int BufLen = 1024;//buffer size
	Socket::byte buffer[BufLen];
	bool ok;
	bool flag = false;
	std::string string_;
	std::string path = "";
	int content = stoi(map["BodyContentLength"]);
	std::string command_ = map["Command"];
	std::string fileName_ = map["FileName"];
	int port = stoi(map["SourcePort"]);
	std::string category = map["Category"];
	std::ofstream* outfile = nullptr;
	bool ostreamFlag = true;
	while (true)
	{
		if (flag == true)//if the some part of buffer from binary file is yet to be received then get the commands again
		{
			string_ = socket.recvString();
			std::vector<std::string> vec_ = m.splitting(string_, '\n');
			std::map<std::string, std::string> map_ = m.makeMap(vec_);
			command_ = map_["Command"];
			fileName_ = map_["FileName"];
			content = stoi(map_["BodyContentLength"]);
			port = stoi(map_["SourcePort"]);
			category = map["Category"];
		}
		if (ostreamFlag)
			{
				if (map["Command"] == "10" || map["Command"] == "11")
				{
					path = "..\\Client1\\ReceivedFiles\\" + fileName_;
					path = FileSystem::Path::getFullFileSpec(path);
				}
				else
				{
					path = category + "\\" + fileName_;
				}
				outfile = new std::ofstream;
				outfile->open(path, std::ios::out | std::ios::binary);//opens the file in binary mode
				ostreamFlag = false;
			}
		ok = socket.recv(content, buffer);
		if (command_ == "1" || command_ == "11")//if the buffer from binary file is coming for last time
		{
			outfile->write(buffer, content);
			outfile->close();
			Display::displayAcknowledgementToConsole("FileName", fileName_, port);
			return true;
		}
		outfile->write(buffer, content);//writing into file
		if (socket == INVALID_SOCKET || !ok)//if the string is not received properly of socket is invalid then break
			return false;
		flag = true;
	}
	return true;
}




//----< demonstration >------------------------------------------------------
#ifdef TEST_RECIEVER
int main()
{
 /* title("Testing Socket Server", '=');

  try
  {
    Verbose v(true);
    SocketSystem ss;
    SocketListener sl(9080, Socket::IP6);
	Reciever cp;
    sl.start(cp);

    Verbose::show("press key to exit: ", always);
    std::cout.flush();
    std::cin.get();
  }
  catch (std::exception& ex)
  {
    Verbose::show("  Exception caught:", always);
    Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
  }*/
}
#endif

