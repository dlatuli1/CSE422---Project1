#include <iostream>
#include <string>
#include <vector>
#include <signal.h>
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

   void InitSigHandler();        //Inits the signal handlers
   static void HandleSIGNAL(int sig);    //Handles the signals
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
   vector<string> pipedVector; // holds commands - delimited by pipes. keeps arguments space delimited in same slot as command
	bool isPiped;
   static vector<pid_t> ForegroudProcesses;//holds all running
   static vector<pid_t> BackgroudProcesses;
   History* ShellCommandHistory;
   Command* ShellCommand;
 //  ShellControl* ShellController;
};

class ShellControl
{
public:

   ShellControl();// Constructor
   ~ShellControl();// Deconstructor

   void InitSigHandler();        //Inits the sigaction structs
   static void HandleSIGNAL(int sig);    //Handles the SIGINT signal
//    static void HandleSIGQUIT(int sig);   //Handles the SIGQUIT signal
//    static void HandleSIGCONT(int sig);   //Handles the SIGCONT signal
//    static void HandleSIGSTP(int sig);    //Handles the SIGSTP signal
//    static void HandleSIGIGNORE(int sig); //Ignore the rest
   bool CheckPiped();


private:
   static vector<pid_t> ForegroudProcesses;//holds all running
   static vector<pid_t> BackgroudProcesses;
   bool pipedCommand;
//struct sigaction action;
};

