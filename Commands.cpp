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
        cout << "Usage: set W1 W2" << endl;
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
