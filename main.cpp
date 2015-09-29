
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int parse_input_line(vector<string>& argvector);

vector<string> commandHistory;

int main(int argc, char* argv[])
{
	cout << "sish >>";
	vector<string> argvector;
	for (;;)
	{
		parse_input_line(argvector);
		//
		
		// stuff
		
		//
		argvector.clear();
	} // main for loop /
}

int parse_input_line(vector<string>& arg_vector)
{
	string input = "";
	size_t delimiter = 0;
	getline(cin, input);
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
	cout << "sish >>";
	return 0;
}
