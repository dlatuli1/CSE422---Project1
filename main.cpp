#include "SimpleShell.h"

int main(int argc, char* argv[])
{
   SimpleShell* SISH = new SimpleShell();

   for (int i = 1; i < argc - 1; i++)
   {
      char c = argv[i][1];

      switch (c)
      {
      case 'f':
      {
         i++;
         string inputFile = argv[i];
         SISH->SetFileInputMode(true, inputFile);
	 
         for(int j = 1; j <= 1024; j++)
         {
	    vector<string> tempVector;

            //check to see if at end of argv
            if(i+1 == argc) break;

            if(argv[i+1][0] != '-')
            {
               i++;
               tempVector.push_back("set");

               ostringstream convert;
               convert << j;
               tempVector.push_back(convert.str());

               tempVector.push_back(argv[i]);
               SISH->SetFromCommandLine(tempVector);
            }
            else
            {
               break;
            }
         }
         break;
      }
      case 'x':
      {
         //do variable substitution stuff
         break;
      }
      case 'd':
      {
         i++;
         SISH->SetDebugLevel(atoi(argv[i]));
         break;
      }
      default:
         cout << "Invalid command line flag -" << c << endl;
      }
   }

   SISH->ShellLoop();

   return 0;
}
