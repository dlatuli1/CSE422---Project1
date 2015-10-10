#include "SimpleShell.h"

SimpleShell::SimpleShell()
{
   ShellCommandHistory = new History();
   ShellCommand = new Command();

	fileInputMode = false;
	debugLevel = 0;
	variableSubstitution = false;

	ShellCommand->shellStatus == Command::Go;
}

SimpleShell::~SimpleShell()
{
	inputFile.close();
}

void SimpleShell::ShellLoop()
{
        InitSigHandler();
        InitEnvironment();
   for (;;)
   {
		argVector.clear();
    	ParseInputLine();
		CheckPiped();

		if (ShellCommand->shellStatus == Command::Go)
		{
			ShellCommand->shellStatus = ExecuteCommand();
		}

		if(debugLevel >= 1)
		{
			char temp;
			cout << "Press <ENTER> to continue...";
			temp = getchar();
		}

		if (ShellCommand->shellStatus == Command::Pause)
		{
			string unpause;
			getline(cin, unpause);
			continue;
		}
		else if (ShellCommand->shellStatus == Command::Exit)
		{
			break;
		}




   }
}

void SimpleShell::SetFileInputMode(bool fim, string input)
{
	this->fileInputMode = fim;
	if(debugLevel >= 1) cout << "Opening file " << input << endl;
	//inputFile.open(input.c_str());
	inputFile.open("./TestInput.txt");
	if(!inputFile.is_open())
	{
		cout << "Error opening input file, reverting to stdin" << endl;
		fileInputMode = false;
	}
}

int SimpleShell::ParseInputLine()
{
	string input = "";
	size_t delimiter = 0;

	if(!fileInputMode)
	{
		cout << "sish >> ";
		getline(cin, input);
	}
	else
	{
		char fileInput[1024];
		bool readSuccess;
		do
		{
			if(debugLevel >= 1) cout << "Trying to read line" << endl;
			readSuccess = inputFile.getline(fileInput, 1024);
			if(debugLevel >= 1) cout << "Read line \"" << fileInput << "\"" << endl;
		}while(!strcmp(fileInput,"") && readSuccess);
		
		input = fileInput;
		if (inputFile.eof())
		{
			if(debugLevel >= 1) cout << "Reached end of file" << endl;
			ShellCommand->shellStatus = Command::Exit;
		}
	}

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
	VariableSub();
	return 0;
}

// subs in variable values before commands are exectuted - Called by ParseInputLine()
// will erase vector value if found that user is trying to set a special variable to something else
// need to work on ! and ? - values from foreground command and background command.
// may want to look into implications with export/unexport?
// echo will not be effected - is this right? i think so
// History deque will not be effected
void SimpleShell::VariableSub()
{
	size_t variablePlace = 0;
	string varString;
	ostringstream shellPid;
	ostringstream foregroundVal;
	ostringstream backgroundPid;

	shellPid << getppid(); //should it be getpid??
    if (ForegroudProcesses.size()) foregroundVal << ForegroudProcesses[ForegroudProcesses.size()-1]; // does nothing now. look into foreground process history stuff
	if (BackgroudProcesses.size())backgroundPid << BackgroudProcesses[BackgroudProcesses.size()-1];  // does nothing now, look into background process history stuff

	if (argVector[0] != "echo") //  ignore this for echo
	{
		for (unsigned int i = 0; i < argVector.size(); i++)
		{
			variablePlace = argVector[i].find('$');
			if (variablePlace != string::npos)
			{
				varString = argVector[i].substr(variablePlace + 1);//variable value

				//check for shell specials first, if not found then replace with whats found in locals
				switch (argVector[i][variablePlace + 1])
				{
				case '$':
					if ((argVector[0] != "set") || (i == 2)) argVector[i] = shellPid.str();				// getpid(); getppid();?  //replaces with PID if not trying to set $ to local variables
					else
					{
						argVector.erase(argVector.begin() + i);								//erase value that user is trying to set, so set will fail
						cout << "\n Setting Shell Variables $, ?, or ! is not allowed!\n\n";
					}
					break;
				case '?':
					if ((argVector[i] != "set") || (i == 2)) argVector[i] = ShellCommand->localVariable["?"];			// hmm need to work on foreground stuff
					else
					{
						argVector.erase(argVector.begin() + i);
						cout << "\n Setting Shell Variables $, ?, or ! is not allowed!\n\n";
					}
					break;
				case '!':
					if ((argVector[0] != "set") || (i == 2)) argVector[i] = ShellCommand->localVariable["!"];			//hmmx2 need to work on background stuff
					else
					{
						argVector.erase(argVector.begin() + i);
						cout << "\n Setting Shell Variables $, ?, or ! is not allowed!\n\n";
					}
					break;
				default:
					if ((ShellCommand->localVariable.find(varString)) != (ShellCommand->localVariable.end()))		// search for variable name in locals
					{
						argVector[i].replace(variablePlace, string::npos, ShellCommand->localVariable[varString]);	//replace $variable in argVector with localVar value
					}
					else
					{
						cout << "\nNo Local Variable -" << varString << "- exists\n"; //couldnt find anything
					}
				}
			}
		}
	}
	return;
}

void SimpleShell::InitEnvironment()
{
    char * homeDir = getenv("PWD");
    char * pathEnv = getenv("PATH");
    string toMap(homeDir);
    string pathMap(pathEnv);
    toMap += "/sish";
    ShellCommand->environment["shell"] = toMap;
    ShellCommand->environment["PATH"] = pathMap;
    ShellCommand->environment["parent"] = toMap;
    setenv("parent",toMap.c_str(), 1);


    return;
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
   else if (argVector[0] == "exit")
   {
	   ((CommandEXIT*)ShellCommand)->Execute(argVector);
	   return Command::Exit;
   }
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
      ((CommandEXTERNAL*)ShellCommand)->Execute(argVector);
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
    //std::cout << "\n\n Signal Caught\n\nsish>>";
    fflush(stdout);
    std::cout << "\nsish>>";
    if(sig == SIGINT||SIGQUIT||SIGCONT||SIGSTOP) // only need to pass these signals to foreground processes
    {
        for(unsigned int i = 0;i < ForegroudProcesses.size();i++)
        {
            kill(ForegroudProcesses[i], sig);//pass on the caught signal to all foreground
        }
    }
}

/*
-    Check Piped will look for '|' as a delimiter
-    each command and all its arguments will exist in the same element - space delimited
-    returns true if it found a pipe
-*/
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
