#include <string>
#include <vector>
#include <deque>

using namespace std;

static deque<string> commandHistory;

int execute_command(vector<string>& argvector);
int parse_input_line(vector<string>& argvector);
bool built_in_command(string arg);
