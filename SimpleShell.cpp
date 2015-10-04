#include "SimpleShell.h"

SimpleShell::SimpleShell()
{
   ShellCommandHistory = new History();
   ShellCommand = new Command();
//   ShellController = new ShellControl();
}

SimpleShell::~SimpleShell()
{
}

void SimpleShell::ShellLoop()
{
   for (;;)
   {
		argVector.clear();
        InitSigHandler();

    	ParseInputLine();
		CheckPiped();

		ShellCommand->shellStatus = ExecuteCommand();

		if (ShellCommand->shellStatus == Command::Pause)
		{
			string unpause;
			getline(cin, unpause);
			continue;
		}




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

Command::ShellStates SimpleShell::ExecuteCommand()
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
   else if (argVector[0] == "pause")    return ((CommandPAUSE*) ShellCommand)->Execute(argVector);
   else if (argVector[0] == "history")  ((CommandHISTORY*) ShellCommand)->Execute(argVector, ShellCommandHistory);
   else
   {
      cout << "EXTERNAL COMMAND" << endl;
   }

   return Command::Go;
}



std::vector<pid_t> SimpleShell::ForegroudProcesses;
std::vector<pid_t> SimpleShell::BackgroudProcesses;

void SimpleShell::InitSigHandler()
{

    signal(SIGINT, HandleSIGNAL);
    signal(SIGQUIT, HandleSIGNAL);
    signal(SIGCONT, HandleSIGNAL);
    signal(SIGSTOP, HandleSIGNAL);
    signal(SIGALRM, HandleSIGNAL);
    signal(SIGABRT, HandleSIGNAL);
    signal(SIGHUP, HandleSIGNAL);
    signal(SIGTERM, HandleSIGNAL);
    signal(SIGUSR1, HandleSIGNAL);
    signal(SIGUSR2, HandleSIGNAL);
}
void SimpleShell::HandleSIGNAL(int sig)
{
    std::cout << "\n\n Signal Caught\n\nsish>>";
    fflush(stdout);
    if(sig == SIGINT||SIGQUIT||SIGCONT||SIGSTOP) // only need to pass these signals to foreground processes
    {
        for(unsigned int i = 0;i < ForegroudProcesses.size();i++)
        {
            kill(ForegroudProcesses[i], sig);//pass on the caught signal to all foreground
        }
    }
}

/*
    Check Piped will look for '|' as a delimiter
    each command and all its arguments will exist in the same element - space delimited
    returns true if it found a pipe
*/
bool SimpleShell::CheckPiped()
{
	unsigned int pipePlace = 0;
	isPiped = false;
	pipedVector.clear();
	if (argVector.size())
	{
		pipedVector.push_back("");
		for (unsigned int i = 0; i < argVector.size(); i++)
		{
			if (argVector[i].find('|') != string::npos)
			{
				isPiped = true;
				pipePlace++;
				pipedVector.push_back("");
			}
			else
			{
				if(pipedVector[pipePlace]!= "") pipedVector[pipePlace].append(" ");
				pipedVector[pipePlace].append(argVector[i]);
			}

		}
	}

	return isPiped;
}

