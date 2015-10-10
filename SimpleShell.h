#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <fstream>
#ifndef History_h
#define History_h
#include "History.h"
#endif
#include "Commands.h"

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

   void SetFileInputMode(bool fim, string input);
   void SetDebugLevel(int dl);
   void SetVariableSubstitution(bool vs);

   void InitSigHandler();        //Inits the sigaction structs
   void InitEnvironment();
   static void HandleSIGNAL(int sig);    //Handles the SIGINT signal
   void VariableSub();
   bool CheckPiped();

private:
   // *************************
   // *** Private functions ***
   // *************************

   // Parse Input Line
   int ParseInputLine();

   // Execute Command
   Command::ShellStates ExecuteCommand();

   // **********************
   // *** Private fields ***
   // **********************
   vector<string> argVector;
   vector<string> pipedVector;// holds commands - delimited by pipes. keeps arguments space delimited in same slot as command
   bool isPiped;
   static vector<pid_t> ForegroudProcesses;//holds all running
   static vector<pid_t> BackgroudProcesses;
   History* ShellCommandHistory;
   Command* ShellCommand;

   bool fileInputMode;
   ifstream inputFile;

   int debugLevel;
   bool variableSubstitution;
};
