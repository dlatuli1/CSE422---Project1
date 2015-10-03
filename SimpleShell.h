#include <iostream>
#include <string>
#include <vector>

#ifndef History_h
#define History_h
#include "History.h"
#endif 
#include "Command.h"

using namespace std;

class SimpleShell
{
public:
   // ************************
   // *** Public functions ***
   // ************************

   // Constructor
   SimpleShell();

   // Deconstructor
   ~SimpleShell();

   // Main Loop
   void ShellLoop();

private:
   // *************************
   // *** Private functions ***
   // *************************

   // Parse Input Line
   int ParseInputLine();

   // Execute Command
   int ExecuteCommand();

   // **********************
   // *** Private fields ***
   // **********************
   vector<string> argVector;
   int shellStatus;

   History* ShellCommandHistory;
   Command* ShellCommand;
};
