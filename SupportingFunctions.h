#include <stdlib.h>
#include <string>

using namespace std;

// this function converts string to char*
char *convert(const string & s)
{
    char *pc = new char[s.size()+1];
    strcpy(pc, s.c_str());
    return pc;
}