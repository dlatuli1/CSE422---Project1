

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

deque<string> commandHistory;
int parse_input_line(vector<string>& argvector);
int history(string historyStr);
bool built_in_command(string arg);

int main(int argc, char* argv[])
{
	//cout << "sish >>";
	vector<string> argvector;
	for (;;)
	{
		
		parse_input_line(argvector);


		if (argvector[0] == "history")
		{
			history(argvector[1]);
		}


		argvector.clear();
		
	} // main for loop /
}


int parse_input_line(vector<string>& arg_vector)
{
	string input = "";
	size_t delimiter = 0;

	cout << "sish >>";
	getline(cin, input);

	if (commandHistory.size() >= 100) //limit history deque size to 100
	{
		commandHistory.erase(commandHistory.begin());
	}
	commandHistory.push_back(input);
	
	while(delimiter!=string::npos)
	{
		delimiter = input.find(' ');
		if (delimiter != string::npos)
		{
			arg_vector.push_back(input.substr(0, delimiter));
			input.erase(0, delimiter+1);
		}
		else
		{
			arg_vector.push_back(input.substr(0, string::npos));

		}
	}
	return 0;
}




// implemeting history command
// should be pretty close, TODO - no error handling other than isdigit
int history(string historyStr)
{

	int lineNum = 0, historyNum = 0;
	
	if (isdigit(historyStr[0])) historyNum = atoi(historyStr.c_str());
	else return 1;
	if (historyNum < commandHistory.size())
	{
		for (unsigned int i = (commandHistory.size() - historyNum - 1); i < commandHistory.size() - 1; i++)  // '-1's are so current history command is not printed to console;
		{
			++lineNum;
			cout << '\t' << lineNum << "  " << commandHistory[i] << '\n';
		}
	}
	else
	{
		//error stuff?
	}
	return 0;
}


// verify if argument is a built-in command
bool built_in_command(string arg) {
    
    // array of built-in commands
    static const string built_in_commands_array[] = {"exit", "clr", "echo", "show", "environ", "export", "unexport", "set", "unset", "dir", "chdir", "history", "repeat", "kill", "fork", "pause", "wait", "help"};
    // make a vector of built-in commands using the above array
    vector<string> built_in_commands (built_in_commands_array, built_in_commands_array + sizeof(built_in_commands_array) / sizeof(built_in_commands_array[0]) );
    if (find(built_in_commands.begin(), built_in_commands.end(), arg) != built_in_commands.end())
        return true;
    else
        return false;
}



//STUFF FROM SLIDES

/*		if built_in_command(arg_vector[0])
		{
			do_it(arg_vector);
			continue;
		} //built_in command
		pathname = find_path(arg_vector[0]);
		if ((pid = fork()) == 0)
		{
			// code executed by child
			if (!ispiped)
			{
				if (stdout_redirect)
				{ // open target file and create it if needed
					fd = open(outfile, O_WRONLY | O_CREAT, 0644);
					close(1); // close stdout/
					dup(fd); // dup into stdout
					close(fd); // good practice
				}
				if (stdin_redirect)
				{
					fd = open(infile, O_RDONLY);
					close(0); // close stdin/
					dup(fd); // dup into stdin
					close(fd); // good practice
				}
				execve(pathname, arg_vector, envp);
				exit(1); // in case execve fails 
			}
			else exec_pipe();
		} //child process
		if (interactive())
		{
			while (wait(0) != pid);
		}*/
	


/*
exec_pipe() {
	int pd[2];
	pipe(pd); //creates the pipe
	...
		if ((pid = fork()) == 0) {
			// child does producer/
			close(1); // close stdout
			dup(pd[1]); // dup into stdout
			close(pd[0]); close(pd[1]); //
			goody
				execve(leftsidecommand, p_argv,
					p_envp); exit(1); // should execve fail
		} //* end of child
		  // parent does consumer
	close(0); // close stdin
	dup(pd[0]; // dup into stdin
	close(pd[0]); close(pd[1]); //
	goody
		execve(rightsidecommand, c_argv,
			c_envp); exit(2); // if execve fails
}*/
