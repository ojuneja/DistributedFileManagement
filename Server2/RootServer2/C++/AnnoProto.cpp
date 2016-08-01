
#include "ActionsAndRules.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include <iostream>

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-') << "\n\n";
    try
    {
  	  Toker toker(argv[i]);
      toker.returnComments();
      SemiExp se(&toker);
      se.makeCommentSemiExp();
      FunctionDefinition fd;
      PrintFunction pf;
      fd.addAction(&pf);

      while(se.getSemiExp())
      {
        IRule::updateLineCount(se);
        fd.doTest(se);
//        if(fd.doTest(se))
//        {
//          std::cout << se.showSemiExp().c_str() << std::endl;
//        }
      }
      std::cout << "\n\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }
}
