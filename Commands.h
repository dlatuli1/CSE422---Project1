#include <vector>
#include <string>

using namespace std;

int show(vector<string> argVector);
int set(vector<string>argVector);
int unset(vector<string>argVector);
int exportVariables(vector<string>argVector);
int unexport(vector<string>argVector);
int environCommand(vector<string>argVector);
int chdir(vector<string>argVector);
int exit(vector<string>argVector);
int wait(vector<string>argVector);
int clr(vector<string>argVector);
int dir(vector<string>argVector);
int echo(vector<string>argVector);
int help(vector<string>argVector);
int pause(vector<string>argVector);
int history(string historyStr);
