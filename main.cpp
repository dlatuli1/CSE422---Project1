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
