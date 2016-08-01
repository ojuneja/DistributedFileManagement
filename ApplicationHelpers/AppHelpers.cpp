/////////////////////////////////////////////////////////////////////////
// AppHelpers.cpp - I/O and Conversion helpers                         //
// ver 2.0                                                             //
//---------------------------------------------------------------------//
// Author: Ojas Juneja                                           //
//---------------------------------------------------------------------//
// Application: OOD Projects #3, #4                                    //
// Platform:    Visual Studio 2013, Dell 2720, Win 8.1 pro             //
/////////////////////////////////////////////////////////////////////////

#include "AppHelpers.h"
#include <string>
#include <iostream>

//using namespace ApplicationHelpers;

bool ApplicationHelpers::Verbose::showState_ = false;
const bool ApplicationHelpers::always = true;

std::mutex mtx;
std::unique_lock<std::mutex> iolock(mtx, std::defer_lock);

void ApplicationHelpers::title(const std::string& msg, char underline)
{
  iolock.lock();
  std::cout << "\n  " << msg;
  std::cout << "\n " << std::string(msg.size() + 2, underline);
  iolock.unlock();
}

void ApplicationHelpers::putLine(const std::string& msg) 
{
  iolock.lock();
  std::cout << "\n" + msg; 
  iolock.unlock();
}

ApplicationHelpers::Verbose::Verbose(bool showState) 
{ 
  showState_ = showState; 
  if (showState)
    putLine("  Verbose mode turned on");
}

void ApplicationHelpers::Verbose::show(const std::string& msg, bool always)
{
  /*if (always == true || showState_ == true)
  {
    std::string temp = std::string("\n  ") + msg;
    iolock.lock();
    std::cout << temp;
    std::cout.flush();
    iolock.unlock();
  }*/
}

void ApplicationHelpers::Verbose::showMap(std::string str, std::map<std::string, std::string> map, bool always)
{
	//if (always == true || showState_ == true)
	//{
	//	iolock.lock();
	//	std::cout << "\n""\n" + str + "\n";
	//	for (auto elem : map)
	//	{
	//		std::cout << elem.first << " : " << elem.second <<"\n";
	//	}
	//	std::cout << "\n";
	//	std::cout.flush();
	//	iolock.unlock();
 // }
}

#ifdef TEST_APPLICATIONHELPERS

int main()
{
	std::cout << "Testing Function of Verbose:\n";
	std::map<std::string, std::string> map;
	map["0"] = "value";
	ApplicationHelpers::Verbose::show("Hello Verbose",true);
	ApplicationHelpers::Verbose::showMap("Displaying Map:",map, true);
}

#endif
