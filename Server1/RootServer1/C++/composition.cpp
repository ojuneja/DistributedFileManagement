/////////////////////////////////////////////////////////////////////
//  composition.cpp - implementing composition relationship        //
//                                                                 //
//  Jim Fawcett, CSE687 - Object Oriented Design, Spring 2002      //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

class part {

  public:
    part(const std::string &name);
    void showPart();

  private:
    std::string _name;
};


part::part(const std::string &name) : _name(name) { }

void part::showPart() {

  std::cout << "\n    I am " << _name << ", a part of whole object";
}

class whole {

  public:
    whole(const std::string &name);
    void show();

  private:
    std::string _name;
    part a;
    part b;
};


whole::whole(const std::string &name) 
     : _name(name), a("Fred"), b("Jake") { }

void whole::show() {

  std::cout << "\n  I am " << _name << ", an object of the whole class.";
  a.showPart();
  b.showPart();
}

void main() {

  whole comp("composition");
  comp.show();
  std::cout << "\n\n";
}