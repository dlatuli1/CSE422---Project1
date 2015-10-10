#include "SimpleShell.h"

int main(int argc, char* argv[])
{
   SimpleShell* SISH = new SimpleShell();
   
   for (int i = 1; i < argc-1; i++)
   {
      char c = argv[i][1];

      switch (c)
      {
          case 'f':
             //do file input stuff
             i++;
             break;
          case 'x':
             //do variable substitution stuff
             break;
          case 'd':
             //do debug level stuff
             i++;
             break;
          default:
             cout << "Invalid command line flag -" << c << endl;
       }
    }

   SISH->ShellLoop();

   return 0;
}
