#include "SimpleShell.h"

int main(int argc, char* argv[])
{
   SimpleShell* SISH = new SimpleShell();
   
   SISH->ShellLoop();

   return 0;
}
