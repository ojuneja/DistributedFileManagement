/////////////////////////////////////////////////////////////////////
// QServer.cpp - serves out references to a static queue           //
//                                                                 //
//     For each type T and for each integer qNum, we get           //
//     one QServer  class and one shared queue.                    //
//                                                                 //
// Ojas Juneja, Syracuse University         //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include "QServer.h"
using namespace std;

void PopAndShow(BlockingQueue<double>& q)
{
  long save = cout.flags();    // save format state
  cout.flags(ios::showpoint);  // show decimal point
  cout.precision(3);           // show only 3 digits on right of decimal
  double item = q.deQ();
  cout << "\n  " << setw(4) << item << " : ";
  cout << "Q size = " << q.size();
  cout.flags(save);            // restore format state
}

int main()
{
  cout << "\n  Demonstrating Queue Server holding Static BlockingQueue "
       << "\n =========================================================";

  // get shared queue

  BlockingQueue<double> &q1 = QServer<double,1>().get();
  q1.enQ(1.0);
  q1.enQ(1.5);
  q1.enQ(2.0);

  // get shared queue again

  BlockingQueue<double> &q2 = QServer<double,1>().get();
  q2.enQ(2.5);
  q2.enQ(3.0);
  q2.enQ(3.5);

  ///////////////////////////////////////////////////////////////////
  // QServer<double,2> is different class than QServer<double,1>
  // so static queue member is not shared between them.

  // get unshared queue

  BlockingQueue<double> &q3 = QServer<double,2>().get();
  q3.enQ(-2.5);
  q3.enQ(-3.0);
  q3.enQ(-3.5);

  cout << "\n  ";
  while(q1.size() > 0)
    PopAndShow(q1);
  cout << "\n\n";

  while(q3.size() > 0)
    PopAndShow(q3);
  cout << "\n\n";
  return 0;
}