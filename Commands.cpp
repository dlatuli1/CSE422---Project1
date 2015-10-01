#include "Commands.h"
#include "main.h"
#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>

map<string, string*> localVariable;

int show(vector<string> argVector)
{
   //Execute show command
    for (int i = 1; i < argVector.size(); i++) {
        cout << argVector[i] << endl;
    }
   return 0;
}

int set(vector<string> argVector)
{
   //Execute set command
    if (argVector.size() == 3) {
        localVariable[argVector[1]] = new string();
        localVariable[argVector[1]] = &argVector[2];
    } else {
        cout << "Usage: set W1 W2" << endl;
    }
   return 0;
}

int unset(vector<string> argVector)
{
   //Execute unset command
    if (argVector.size() == 2) {
        if (localVariable[argVector[1]]) {
            delete localVariable[argVector[1]];
        }
    } else {
        cout << "Usage: unset W" << endl;
    }
   return 0;
}

int exportVariable(vector<string>argVector)
{
   //Execute export command
    if (argVector.size() == 3) {
        // since setenv() takes char* type, we have to first convert the arguments into char*
        char *cStringArg1 = new char[argVector[1].length() + 1];
        char *cStringArg2 = new char[argVector[2].length() + 1];
        strcpy(cStringArg1, argVector[1].c_str());
        strcpy(cStringArg2, argVector[2].c_str());
        setenv(cStringArg1, cStringArg2, true);
        delete cStringArg1;
        delete cStringArg2;
    } else {
        cout << "Usage: export W1 W2" << endl;
    }
   return 0;
}

int unexport(vector<string>argVector)
{
   //Execute unexport command
    if (argVector.size() == 2) {
        // since unsetenv() takes char* type, we have to first convert the arguments into char*
        char *cStringArg = new char[argVector[1].length() + 1];
        strcpy(cStringArg, argVector[1].c_str());
        unsetenv(cStringArg);
        delete cStringArg;
    } else {
        cout << "Usage: unexport W" << endl;
    }
   return 0;
}

int environCommand(vector<string>argVector)
{
   //Execute environ command
   return 0;
}

int chdir(vector<string>argVector)
{
   //Execute chdir command
   return 0;
}

int exit(vector<string>argVector)
{
   //Execute exit command
   return 0;
}

int wait(vector<string>argVector)
{
   //Execute wait command
   return 0;
}

int clr(vector<string>argVector)
{
   //Execute clr command
   return 0;
}

int dir(vector<string>argVector)
{
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

int echo(vector<string>argVector)
{
	//Execute echo command
	for (unsigned int i = 1; i < argVector.size(); i++)
	{
		cout << argVector[i] << ' ';
	}
	cout << '\n';
	return 0;
}

int help(vector<string>argVector)
{
	//Execute help command
	return 0;
}

int pause(vector<string>argVector)
{
	//Execute pause command

	cout << "\n------------------------------\n\tsish paused:\n\tPress Enter to resume\n------------------------------\n";

	//either put the waiting loop here, 
	//or expand on the returns for Shell operation functions like pause and exit

	return 1;
}

//history command
int history(vector<string> argvector)
{
	unsigned int count = 0, history_asked = 0;
	if (argvector.size() > 1)
	{
		if (isdigit(argvector[1][0])) history_asked = atoi(argvector[1].c_str());  //converting string parameter to int
	}

	if ((history_asked >= ShellCommandHistory::HistorySize()) || (history_asked == 0))
	{
		history_asked = ShellCommandHistory::HistorySize() - 1; //trimming max history asked to current size
	}

	for (unsigned int i = (ShellCommandHistory::HistorySize() - history_asked - 1); i < ShellCommandHistory::HistorySize() - 1; i++)  // '-1's are so current history command is not printed to console;
	{
		cout << '\t' << ++count << "  " << ShellCommandHistory::Get(i) << '\n';
	}
	return 0;
}

