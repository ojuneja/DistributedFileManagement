#include"Reciever.h"

///////////////////////////////////////////////////////////////////
//  Receiver.cpp      -  implementation file for Receiver class  //
//  ver 1.4                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application: Project - 4(Remote Code Management)             //
//                jfawcett@twcny.rr.com                          //
// Author:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

//callable object to receive string or byte buffer
void Reciever::operator()(Socket& socket_)
{
	Message m;
	bool flag = false;
  while (true)
  {
    // interpret test command
    std::string command = socket_.recvString();
	std::string header = "";
	std::stringstream ss(command);
	std::string item;
	std::vector<std::string> vec = m.splitting(command,'\n');
	std::map<std::string, std::string> map = m.makeMap(vec);
	std::string mapCommand = map["Command"];
	std::string fileName = map["FileName"];
	if (command.size() == 0)//if there is no command
	{
		break;
	}
	else
	{
		performOperations(socket_, m,command, mapCommand, map);
	}
  }
}

//performs operations according to HTTP Message Header
void Reciever::performOperations(Socket &socket_,Message m,std::string command, std::string mapCommand, std::map<std::string, std::string> map)
{
	std::string fileName = map["FileName"];
	
	if ((mapCommand == "0" || mapCommand == "1") && fileName.size() >0)//if the contents from binary/text file is received
	{
		handleFile(socket_, map);
	}
	else if (mapCommand == "4")//if the file or string is uploaded by client
	{
		Display::displayString("File: '" + map["FileName"] + "' is uploaded by Client");
		queue_.enQ("File: " + map["FileName"] + " uploaded at Path: " + map["Category"] +  "\\" +map["FileName"]);
	}
	else if (mapCommand == "5")
	{
		Display::displayString("File is downloaded at '" + FileSystem::Path::getFullFileSpec(map["FileName"]) + "' by Client");
		queue_.enQ("File: " + map["FileName"] + " downloaded at Path: " + FileSystem::Path::getFullFileSpec(map["Category"] + map["FileName"]));
	}
	else if ((mapCommand == "6" || mapCommand == "9") && fileName.length() > 0)
	{
		fileAndTextSearching(map);
	}
	else if (mapCommand == "7")
	{
		ackFileSearch(m, map);
	}
	else if (mapCommand == "8")
	{
		Message m("DOWNLOAD", "localhost", "localhost", 0, stoi(map["SourcePort"]), stoi(map["DestinationPort"]), map["FileName"], map["Category"]);
		queue_.enQ(m.makeHeader());
	}
	else if (mapCommand == "10" || mapCommand == "11")
	{
		handleFile(socket_, map);
	}
}

//process file and text searching if a command related to searching comes
void Reciever::fileAndTextSearching(std::map<std::string,std::string> map)
{
	std::string filePaths = "";
	if (map["Command"] == "9")
	{
		filePaths = search(map["FileName"], true);
	}
	else
	{
		filePaths = search(map["FileName"]);
	}
	Message m("LIST", "localhost", "localhost", 0, stoi(map["DestinationPort"]), stoi(map["SourcePort"]), map["DestinationPort"] + "|" + filePaths);
	queue_.enQ(m.makeHeader());
}

//process file serch acknowledgement if suitable command comes
void Reciever::ackFileSearch(Message m ,std::map<std::string, std::string> map)
{
	Display::displayString("Demonstrating Requirement 5, 6, 7 by showing Name of Following Files to Client specified by input : ");
	Display::displayToConsole(map["FileName"], '|');
	queue_.enQ(map["FileName"]);
}

//searches text in a file and also search directories by calling function of FileManager/FileSearch class
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
	Display::displayString("Demonstrating Requirement 5, 6, 7 by Searching following Files on Server : ");
	Display::displayToConsole(list, '|');
	return list;
	
}

//handles file by using file upload fucntion to write buffer into file
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
		if (map["Command"] == "0" || map["Command"] == "1")
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

//write buffer into specified file
bool Reciever::uploadingFile(Socket& socket, std::map<std::string,std::string> map)
{
	std::string string_;
	int content = stoi(map["BodyContentLength"]);
	std::string command_ = map["Command"];
	std::string fileName_ = map["FileName"];
	int port = stoi(map["SourcePort"]);
	std::string category = map["Category"];
	return uploadingFileOperation(socket,string_, command_, content, fileName_, port, category);
}

bool Reciever::uploadingFileOperation(Socket& socket,std::string string_, std::string command_, int content, std::string fileName_, int port, std::string category)
{
	Message m;
	std::string path = "";
	const int BufLen = 1024;//buffer size
	Socket::byte buffer[BufLen];
	bool ok = false, flag = false, ostreamFlag = true;
	std::ofstream* outfile = nullptr;
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
			category = map_["Category"];
		}
		if (ostreamFlag)
		{
			path = setPath(command_, category,fileName_);
			outfile = new std::ofstream;
			outfile->open(path, std::ios::out | std::ios::binary);//opens the file in binary mode
			ostreamFlag = false;
		}
		ok = socket.recv(content, buffer);
		if (command_ == "1" || command_ == "11")//if the buffer from binary file is coming for last time
		{
			outfile->write(buffer, content);
			outfile->close();
			return true;
		}
		outfile->write(buffer, content);//writing into file
		if (socket == INVALID_SOCKET || !ok)//if the string is not received properly of socket is invalid then break
			return false;
		flag = true;
	}
	return true;
}

//set path where uploading file should be done
std::string Reciever::setPath(std::string command_, std::string category, std::string fileName_)
{
	std::string path = "";
	if (command_ == "10" || command_ == "11")
	{
		path = ".\\Client1\\ReceivedFiles\\" + fileName_;
		path = FileSystem::Path::getFullFileSpec(path);
	}
	else
	{
		path = category + "\\" + fileName_;
	}
	return path;
}

//----< demonstration >------------------------------------------------------
#ifdef TEST_RECIEVER
int main()
{
  title("Testing Socket Server", '=');

  try
  {
 
	std::cout << "\n"<<"All files will be saved in ..\\Reciever\\RecievedFiles" << "\n\n";
	std::cout << "\n" << "Please check appropriate Folder to check whether file is uploaded." << "\n";
    SocketSystem ss;
    SocketListener sl(9080, Socket::IP6);
	BlockingQueue<std::string> queue1;
	Reciever cp(queue1);
    sl.start(cp);
    std::cout.flush();
    std::cin.get();
  }
  catch (std::exception& ex)
  {
	  std::cout << ex.what();
    /*Verbose::show("  Exception caught:", always);
    Verbose::show(std::string("\n  ") + ex.what() + "\n\n");*/
  }
}
#endif

