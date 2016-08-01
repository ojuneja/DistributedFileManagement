///////////////////////////////////////////////////////////////////
//  Display.cpp      -  header file for Display class            //
//  ver 1.1                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  Application:Message,Project - 4(Remote Code Management)       //
// Author:   Ojas Juneja                                    //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

#include"Display.h"

//display connection estalished to console
void Display::displayConnectionToConsole(std::string string, int port)
{
	if (string.find("Sender") != std::string::npos) //if the client is connected to port then display below message
		std::cout << "\n" + string + " connected to port: " + (std::to_string)(port) + "\n";
	else
		std::cout << "\n" + string + " started at port: " + (std::to_string)(port)+"\n";
	
}


//display file status while sending to console
void Display::displayString(std::string type, std::string string, int bytes, int port,std::string fileAddress)
{
	if (type == "Uploading") // if the sender is uploading file then display below message
		std::cout << "\n" + type + " File: " + string + " present in address: " + fileAddress + " of size: " + (std::to_string)(bytes)+" bytes to Server listening at port: " + (std::to_string)(port)+"\n";
	else
		std::cout << "\n" + type + " Text string: " + string + " of size: " + (std::to_string)(bytes)+" bytes to Server listening at port: " + (std::to_string)(port)+"\n";
	
}

//display message to console
void Display::displayString(std::string string)
{
	std::cout << "\n " + string + "\n";
}

//dislays number of files found to console
void Display::displayToConsole(std::string list, char seperator)
{
	std::vector<std::string> elements;
	if (list.find("|") != std::string::npos)
	{
		std::stringstream ss(list);
		std::string item;
		while (std::getline(ss, item, seperator)) {
			elements.push_back(item);
		}
	}
	else
		elements.push_back(list);
	for (auto elem : elements)
	{
		std::cout << elem << "\n";
	}
}

#ifdef TEST_DISPLAY

int main()
{
	//testing functionalities of each file
	Display::displayString("Testing Display Function one by one:");
	Display::displayConnectionToConsole("Sender",8089);
	Display::displayConnectionToConsole("Receiver", 9000);
	Display::displayString("Uploading", "Demo.txt", 1024, 9000);
	Display::displayString("Sending", "abcde", 1024, 9000);
	return 0;
}

#endif

