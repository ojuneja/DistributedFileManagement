#ifndef APPHELPERS_H
#define APPHELPERS_H
/////////////////////////////////////////////////////////////////////////
// AppHelpers.h - I/O and Conversion helpers                           //
// ver 2.0                                                             //
//---------------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                      //
// All rights granted provided this copyright notice is retained       //
//  Modified By: Ojas Juneja                                          //
//---------------------------------------------------------------------//
// Application: OOD Projects #3, #4                                    //
// Platform:    Visual Studio 2013, Dell 2720, Win 8.1 pro             //
/////////////////////////////////////////////////////////////////////////
/*  Maintenance History :
*--------------------
*  ver 1.0 : 10 Apr 15 - made one more function void static showMap(std::string str,std::map<std::string, std::string> map, bool always)*/
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>
#include<map>
namespace ApplicationHelpers
{
  void title(const std::string& msg, char underline = '-');

  template<typename T>
  std::string toString(T t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

  void putLine(const std::string& msg="");

  extern const bool always;

  class Verbose
  {
  public:
    Verbose(bool showState = true);
    void static show(const std::string& msg, bool always = false);
	void static showMap(std::string str,std::map<std::string, std::string> map, bool always);
  private:
    static bool showState_;
  };
}

#endif
