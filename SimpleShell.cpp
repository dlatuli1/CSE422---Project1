#include "SimpleShell.h"

SimpleShell::SimpleShell()
{
   ShellCommandHistory = new History();
   ShellCommand = new Command();
}

SimpleShell::~SimpleShell()
{
}

void SimpleShell::ShellLoop()
{
   for (;;)
   {
    	ParseInputLine();

		shellStatus = ExecuteCommand();
		
		if (shellStatus == 1) 
		{
			string unpause;
			getline(cin, unpause);
			continue;
		}


      argVector.clear();

   }
}

int SimpleShell::ParseInputLine()
{
	string input = "";
	size_t delimiter = 0;

	cout << "sish >> ";
	getline(cin, input);

	if (ShellCommandHistory->HistorySize() >= 100) ShellCommandHistory->TrimOld(); //limit history deque size to 100

	ShellCommandHistory->PushNew(input);

	while (delimiter != string::npos)
	{
		delimiter = input.find(' ');
		if (delimiter != string::npos)
		{
			argVector.push_back(input.substr(0, delimiter));
			input.erase(0, delimiter + 1);
		}
		else argVector.push_back(input.substr(0, string::npos));
	}
	return 0;
}

int SimpleShell::ExecuteCommand()
{
   if      (argVector[0] == "show")     ((CommandSHOW*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "set")      ((CommandSET*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "unset")    ((CommandUNSET*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "export")   ((CommandEXPORT*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "unexport") ((CommandUNEXPORT*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "environ")  ((CommandENVIRON*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "chdir")    ((CommandCHDIR*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "exit")     ((CommandEXIT*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "wait")     ((CommandWAIT*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "clr")      ((CommandCLR*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "dir")      ((CommandDIR*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "echo")     ((CommandECHO*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "help")     ((CommandHELP*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "pause")    ((CommandPAUSE*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "history")  ((CommandHISTORY*) ShellCommand)->Execute(argVector, ShellCommandHistory);
   else
   {
      cout << "EXTERNAL COMMAND" << endl;
   }

   return 0;
}
