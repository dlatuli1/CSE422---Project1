#include "Commands.h"
#include "main.h"
#include <iostream>
#include <map>

map<string string> localVariable;

int show(vector<string> argVector)
{
   //Execute show command
   return 0;
}

int set(vector<string> argVector)
{
   //Execute set command
    if (argVector.size == 3) {
        localVariable[argVector[1]] = new string();
        localVariable[argVector[1]] = argVector[2];
    }
   return 0;
}

int unset(vector<string> argVector)
{
   //Execute unset command
   return 0;
}

int exportVariables(vector<string>argVector)
{
   //Execute export command
   return 0;
}

int unexport(vector<string>argVector)
{
   //Execute unexport command
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
   return 0;
}

int echo(vector<string>argVector)
{
   //Execute echo command
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
   return 0;
}

// implemeting history command
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
