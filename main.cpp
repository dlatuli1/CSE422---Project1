#include <iostream>
#include <algorithm>

#include "main.h"

using namespace std;

int main(int argc, char* argv[])
{
   vector<string> argvector;
   
	//Temporary variable to get concept into code -- may want some way to return status to main from commands like pause, exit, etc. 
	//for now im just setting shellStatus to 1 on pause(definitely not final way), and waiting in main. 
	//I can also do the wait-loop within the command -- let me know what you guys think. pretty simple either way
	int shellStatus;
	
   for (;;)
   {
    	parse_input_line(argvector);

		shellStatus = execute_command(argvector);
		
		//see note above
		if (shellStatus == 1) 
		{
			string unpause;
			getline(cin, unpause);
			continue;
		}



      argvector.clear();

   } // main for loop /
}

int execute_command(vector<string>& argVector)
{
   if      (argVector[0] == "show")     show(argVector);
   else if (argVector[0] == "set")      set(argVector);
   else if (argVector[0] == "unset")    unset(argVector);
   else if (argVector[0] == "export")   exportVariables(argVector);
   else if (argVector[0] == "unexport") unexport(argVector);
   else if (argVector[0] == "environ")  environCommand(argVector);
   else if (argVector[0] == "chdir")    chdir(argVector);
   else if (argVector[0] == "exit")     exit(argVector);
   else if (argVector[0] == "wait")     wait(argVector);
   else if (argVector[0] == "clr")      clr(argVector);
   else if (argVector[0] == "dir")      dir(argVector);
   else if (argVector[0] == "echo")     echo(argVector);
   else if (argVector[0] == "help")     help(argVector);
   else if (argVector[0] == "pause")    return pause(argVector);
   else if (argVector[0] == "history")  history(argVector);
   else
   {
      cout << "TBD";
   }

   return 0;
}

int parse_input_line(vector<string>& arg_vector)
{
	string input = "";
	size_t delimiter = 0;

	cout << "sish >>";
	getline(cin, input);

	if (ShellCommandHistory::HistorySize() >= 100) ShellCommandHistory::TrimOld(); //limit history deque size to 100
	ShellCommandHistory::PushNew(input);

	while (delimiter != string::npos)
	{
		delimiter = input.find(' ');
		if (delimiter != string::npos)
		{
			arg_vector.push_back(input.substr(0, delimiter));
			input.erase(0, delimiter + 1);
		}
		else arg_vector.push_back(input.substr(0, string::npos));
	}
	return 0;
}



// verify if argument is a built-in command
bool built_in_command(string arg) {

   // array of built-in commands
   static const string built_in_commands_array[] = { "exit", "clr", "echo", "show", "environ", "export", "unexport", "set", "unset", "dir", "chdir", "history", "repeat", "kill", "fork", "pause", "wait", "help" };
   // make a vector of built-in commands using the above array
   vector<string> built_in_commands(built_in_commands_array, built_in_commands_array + sizeof(built_in_commands_array) / sizeof(built_in_commands_array[0]));
   if (find(built_in_commands.begin(), built_in_commands.end(), arg) != built_in_commands.end())
      return true;
   else
      return false;
}
