///////////////////////////////////////////////////////////////////
//  Message.cpp      -  implmentation file for Message class     //
//  ver 1.2                                                      //
//                                                               //
//  Language:     Visual C++, ver 13.0                           //
//  Platform:     HP Pavilion g series, Win 7.0                  //
//  ApplicationProject - 4(Remote Code Management)               //
// Author:         Ojas Juneja                                   //
//                Syracuse University                            //
//                ojuneja@syr.edu, (315) 751-8944                //
///////////////////////////////////////////////////////////////////

#include"Message.h"


//constructs HTTP Message
std::string Message::makeHeader()
{
	message = "Command~" + h->command_ + "\n" + "SourceIpAddress~" + h->SourceIpAddress_ + "\n" + "DestinationIpAddress~" + h->DestinationIpAddress_ + "\n"
		+ "BodyContentLength~" + (std::to_string)(h->bodycontentlength_) + "\n" + "SourcePort~" + (std::to_string)(h->sourcePort_) + "\n"
		+ "DestinationPort~" + (std::to_string)(h->destinationPort_) + "\n" + "FileName~" + h->filename_ + "\n" + "Category~" + h->category_;
	return message;
}

//constrcuts queue commands
std::string Message::makeQueueCommand(std::string comm, std::string fileName, std::string option, std::string destinationAddress, int port, std::string text)
{
	std::string string_ = "Command~" + comm + "$" + "FileName~" + fileName + "$" + "Option~" + option + "$" + "Text~" + text + "$" + "DestinationAddress~" + destinationAddress + "$" + "DestinationPort~" + (std::to_string)(port);
	return string_;
}

//maps the header title with its value
std::map<std::string, std::string> Message::makeMap(std::vector<std::string> vec)
{
	std::map< std::string, std::string> Map;
	for (auto elem : vec)
	{
		std::vector<std::string> split1 = splitting(elem, '~'); //splitting the elements of vector by using delimeter ":"
		if (elem.size() - 1 != split1[0].size())
			Map[split1[0]] = split1[1];
	}
	return Map;
}

//split message by using delimiter specified by user
std::vector<std::string> Message::splitting(std::string message, char seperator)
{
	std::vector<std::string> elements;
	std::stringstream ss(message);
	std::string item;
	while (std::getline(ss, item, seperator)) {
		elements.push_back(item);
	}
	return elements;
}

#ifdef TEST_MESSAGE
int main()
{
	//Test Stub to test functionality of functions of Message class
	Message m("0", "localhost", "localhost", 1024, 9080, 8807, "Demo.txt");
	std::cout << "HTTP Message header is: " << m.makeHeader() << "\n""\n";
	std::map<std::string, std::string> map = m.makeMap(m.splitting(m.makeHeader(),'\n'));
	for (auto elem : map)
	{
		std::cout << "Key in Map: " << elem.first << " with its Value in Map: "<<elem.second<<"\n";

	}
	std::cout << "\nDisplaying Queue Command: " << m.makeQueueCommand("0", "Demo.txt", "", "localhost", 9089);
	return 0;
}
#endif



