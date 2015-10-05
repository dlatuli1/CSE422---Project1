#include "Commands.h"

Command::Command()
{
}

Command::~Command()
{
}

int Command::Execute()
{
   return 0;
}

int CommandSHOW::Execute(vector<string> argVector)
{
   cout << "Executing show command" << endl;
   //Execute show command
   for (unsigned int i = 1; i < argVector.size(); i++) {
       cout << argVector[i] << endl;
   }
   return 0;
}

int CommandSET::Execute(vector<string> argVector)
{
   cout << "Executing set command" << endl;
   //Execute set command
   if (argVector.size() == 3) {
       localVariable[argVector[1]] = argVector[2];
   } else {
       cout << "Usage: set W1 W2" << endl;
   }
   return 0;
}

int CommandUNSET::Execute(vector<string> argVector)
{
   cout << "Executing unset command" << endl;
   //Execute unset command
   if (argVector.size() == 2)
   {
		localVariable.erase(argVector[1]);
   }
   else 
   {
       cout << "Usage: unset W" << endl;
   }
   return 0;
}

int CommandEXPORT::Execute(vector<string> argVector)
{
   cout << "Executing export command" << endl;
   //Execute export command
   if (argVector.size() == 3) {
       // since setenv() takes char* type, we have to first convert the arguments into char*
       char *cStringArg1 = new char[argVector[1].length() + 1];
       char *cStringArg2 = new char[argVector[2].length() + 1];
       strcpy(cStringArg1, argVector[1].c_str());
       strcpy(cStringArg2, argVector[2].c_str());
       //setenv(cStringArg1, cStringArg2, true);
       delete cStringArg1;
       delete cStringArg2;
   } else {
       cout << "Usage: export W1 W2" << endl;
   }
   return 0;
}

int CommandUNEXPORT::Execute(vector<string> argVector)
{
   cout << "Executing unexport command" << endl;
   //Execute unexport command
   if (argVector.size() == 2) {
       // since unsetenv() takes char* type, we have to first convert the arguments into char*
       char *cStringArg = new char[argVector[1].length() + 1];
       strcpy(cStringArg, argVector[1].c_str());
       //unsetenv(cStringArg);
       delete cStringArg;
   } else {
       cout << "Usage: unexport W" << endl;
   }
   return 0;
}

int CommandENVIRON::Execute(vector<string> argVector)
{
   cout << "Executing environ command" << endl;
   //Execute environ command
   return 0;
}

int CommandCHDIR::Execute(vector<string> argVector)
{
   cout << "Executing chdir command" << endl;
   //Execute chdir command
   return 0;
}

int CommandEXIT::Execute(vector<string> argVector)
{
   cout << "Executing exit command" << endl;
   //Execute exit command
   return 0;
}

int CommandWAIT::Execute(vector<string> argVector)
{
   cout << "Executing wait command" << endl;
   //Execute wait command
   return 0;
}

int CommandCLR::Execute(vector<string> argVector)
{
   cout << "Executing clr command" << endl;
   //Execute clr command
   return 0;
}

int CommandDIR::Execute(vector<string> argVector)
{
   cout << "Executing dir command" << endl;
   	//Execute dir command
	char workingDir_Cstr[FILENAME_MAX];
	vector<string> dirContents;
	GetWorkingDir(workingDir_Cstr, sizeof(workingDir_Cstr));
	string workingDir(workingDir_Cstr);

	cout << workingDir << '\n';

#ifdef WINDOWS
	workingDir = workingDir + "\\*";
	WIN32_FIND_DATA fileData;
	HANDLE hFind;
	hFind = FindFirstFile(workingDir.c_str(), &fileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << "\n Failure accessing directory\n";
		return 0;
	}
	while (FindNextFile(hFind, &fileData))
	{
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			cout << "\t <DIR> " << fileData.cFileName << '\n';
		}
		else
		{
			cout << "\t" << fileData.cFileName << '\n';
		}
	}
#else
	DIR *dp;
	struct dirent *dirp;
	string DIRstr;

	if ((dp = opendir(workingDir.c_str())) == NULL)
	{
		cout << "\n Failure accessing directory\n";
		return 0;
	}
	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_type == DT_DIR)
		{
        		DIRstr = dirp->d_name;
			if ((DIRstr != ".") && (DIRstr != "..")) cout << "\t <DIR> " << dirp->d_name << '\n';
		}
		else
		{
			cout << "\t" << dirp->d_name << '\n';
		}
	}
	closedir(dp);

#endif
	return 0;
}

int CommandECHO::Execute(vector<string> argVector)
{
   cout << "Executing echo command" << endl;
	//Execute echo command
	for (unsigned int i = 1; i < argVector.size(); i++)
	{
		cout << argVector[i] << ' ';
	}
	cout << '\n';
	return 0;
}

int CommandHELP::Execute(vector<string> argVector)
{
   cout << "Executing help command" << endl;
	//Execute help command
	return 0;
}

Command::ShellStates CommandPAUSE::Execute(vector<string> argVector)
{
   cout << "Executing pause command" << endl;
	//Execute pause command

	cout << "\n------------------------------\n\tsish paused:\n\tPress Enter to resume\n------------------------------\n";

	//either put the waiting loop here,
	//or expand on the returns for Shell operation functions like pause and exit

	return  Pause;
}

int CommandHISTORY::Execute(vector<string> argVector, History* ShellCommandHistory)
{
   cout << "Executing history command" << endl;
	unsigned int count = 0, history_asked = 0;
	if (argVector.size() > 1)
	{
		if (isdigit(argVector[1][0])) history_asked = atoi(argVector[1].c_str());  //converting string parameter to int
	}

	if ((history_asked >= ShellCommandHistory->HistorySize()) || (history_asked == 0))
	{
		history_asked = ShellCommandHistory->HistorySize() - 1; //trimming max history asked to current size
	}

	for (unsigned int i = (ShellCommandHistory->HistorySize() - history_asked - 1); i < ShellCommandHistory->HistorySize() - 1; i++)  // '-1's are so current history command is not printed to console;
	{
		cout << '\t' << ++count << "  " << ShellCommandHistory->Get(i) << '\n';
	}
	return 0;
}



