/////////////////////////////////////////////////////////////////////
//  inheritance.cpp - implementing inheritance relationship        //
//                                                                 //
//  Jim Fawcett, CSE687 - Object Oriented Design, Spring 2002      //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

class Base {

  public:
    Base(const std::string &name);
    virtual ~Base() { }
    virtual void show();

  private:
    std::string _name;
};

Base::Base(const std::string &name) : _name(name) { }

void Base::show() {

  std::cout << "\n  I am " << _name << ", an object of the Base class";
}


class Derived : public Base {

  public:
    Derived(const std::string &name);
    virtual void show();

  private:
    std::string _name;
};

Derived::Derived(const std::string &name) : _name(name), Base("Bob") { }

void Derived::show() {

  std::cout << "\n  I am " << _name << ", and use a base object.";
  Base::show();
}


void main() {

  Base Billy("Billy");
  Billy.show();

  std::cout << std::endl;

  Derived Davey("Davey");
  Davey.show();

  std::cout << "\n\n";
}