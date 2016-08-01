/////////////////////////////////////////////////////////////////////
//  using.cpp - implementing using relationships                   //
//                                                                 //
//  Jim Fawcett, CSE687 - Object Oriented Design, Spring 2002      //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

class Usee {

  public:
    Usee(const std::string &name);
    void showUsee();

  private:
    std::string _name;
};

Usee::Usee(const std::string &name) : _name(name) { }

void Usee::showUsee() {

  std::cout << "\n    I am " << _name << ", an object of the Used class";
}


class User {

  public:
    User(const std::string &name);
    void show(Usee &usee);

  private:
    std::string _name;
};

User::User(const std::string &name) : _name(name) { }

void User::show(Usee &usee1) {

  Usee usee2("Jake");
  std::cout << "\n  I am " << _name << ", and use two objects.";
  usee1.showUsee();
  usee2.showUsee();
}


void main() {

  Usee Fred("Fred");
  User user("user");
  user.show(Fred);

  std::cout << "\n\n";
}