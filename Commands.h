#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


#ifdef WINDOWS
#include <direct.h>
#include <Windows.h>
#define GetWorkingDir _getcwd
#else
#include <unistd.h>
#include <dirent.h>
#define GetWorkingDir getcwd
#endif

#ifndef History_h
#define History_h
#include "History.h"
#endif

using namespace std;

class Command
{
private:


public:

   Command();

   ~Command();

   int Execute();
   static map<string, string> localVariable;
   map<string, string> environment;
   enum ShellStates
   {
	   Go,
	   Pause,
	   Wait,
	   Kill,
	   Exit
   };
   ShellStates shellStatus;
};

class CommandSHOW : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandSET : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandUNSET : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandEXPORT : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandUNEXPORT : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandENVIRON : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandCHDIR : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandEXIT : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandWAIT : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandCLR : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandDIR : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandECHO : Command
{
public:
   int Execute(vector<string> argVector);
};

class CommandHELP : Command
{
public:
   int Execute(vector<string> argVector);
};

/*
class CommandREPEAT : Command
{
public:
    int Execute(vector<string> argVector);
};*/

class CommandPAUSE : Command
{
public:
   ShellStates Execute(vector<string> argVector);
};

class CommandHISTORY : Command
{
public:
   int Execute(vector<string> argVector, History* ShellCommandHistory);
};
class CommandKILL : Command
{
public:
    int Execute(vector<string> argVector);
};

class CommandEXTERNAL : Command
{
public:
   int Execute(vector<string> argVector);
};
