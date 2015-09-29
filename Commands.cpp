#include "Commands.h"
#include "main.h"
#include <iostream>

int show(vector<string> argVector)
{
   //Execute show command
   return 0;
}

int set(vector<string>argVector)
{
   //Execute set command
   return 0;
}

int unset(vector<string>argVector)
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
