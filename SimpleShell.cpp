#include "SimpleShell.h"

SimpleShell::SimpleShell()
{
   ShellCommandHistory = new History();
   ShellCommand = new Command();

   fileInputMode = false;
   debugLevel = 0;
   variableSubstitution = false;

   ShellCommand->shellStatus = Command::Go;
}

SimpleShell::~SimpleShell()
{
   if (fileInputMode)
   {
      inputFile.close();
   }
}

void SimpleShell::ShellLoop()
{
   InitSigHandler();
   InitEnvironment();
   for (;;)
   {
      argVector.clear();
      ParseInputLine();

      bool blankCommand = true;
      for (int i = 0; i < argVector.size(); i++)
      {
         if (!(argVector[i] == " ") && !(argVector[i] == "")) blankCommand = false;
      }
      if (blankCommand) continue;

       if (CheckPiped())
       {
           int status;
           int p[2];
           int pid1, pid2;
           
           if(pipe(p) == -1){
               cerr << "Pipe call error" << endl;
               continue;
           }
           
           //For child one
           if((pid1 = fork())== -1){
               cerr << "Error fork child 1" << endl;
               continue;
           }
           else if(pid1 == 0){
               
               if (debugLevel >= 1) cout << "Child 1 excuting..." << endl;
               
               //construct argment list for executable
               char** argList1 = new char* [pipedVector.size()];
               for(int i = 0; i < pipedVector[0].length(); i++){
                   argList1[i] = strdup(pipedVector[0].c_str());
               }
               argList1[pipedVector.size()] = (char*)0;
               
               if(close(p[0]) == -1){
                   cerr << "Error closing read end" << endl;
                   pid1 = getpid();
                   kill(pid1, SIGKILL);
                   continue;
               }
               dup2(p[1], STDOUT_FILENO);
               execvp((pipedVector[0]).c_str(), argList1);
               cerr << "Execv file1 error!" << endl;
               pid1 = getpid();
               kill(pid1, SIGKILL);
               continue;
           }
           
           pid_t w1 = waitpid(pid1, &status, WUNTRACED | WCONTINUED);
           if (debugLevel >= 1) cout << "Wait process 1.." << endl;
           
           //For child two
           pid2 = fork();
           if(pid2 == -1){
               cerr << "Error fork child 2" << endl;
               pid2 = getpid();
               kill(pid2, SIGKILL);
               continue;
           }
           else if(pid2 == 0){
               if (debugLevel >= 1) cout << "Child 2 excuting..." << endl;
               
               //construct argment list for executable
               char** argList2 = new char* [pipedVector[1].length()];
               for(int i = 0; i < pipedVector[1].length() - 1; i++){
                   argList2[i] = strdup((char *)pipedVector[1][i]);
               }
               argList2[pipedVector[1].length() - 1] = (char*)0;
               
               if(close(p[1]) == -1){
                   cerr << "Error closing write end" << endl;
                   pid2 = getpid();
                   kill(pid2, SIGKILL);
                   continue;
               }
               if(dup2(p[0], STDIN_FILENO) == -1){
                   cerr << "STDIN fail!" << endl;
                   pid2 = getpid();
                   kill(pid2, SIGKILL);
                   continue;
               }
               execv(pipedVector[1].c_str(), argList2);
               std::cerr << "Execl executable 2 failed!" << endl;
               pid2 = getpid();
               kill(pid2, SIGKILL);
               continue;
           }
           
           //Go back to parent process
           if (debugLevel >= 1) cout << "Child process waiting..." << endl;
           close(p[0]);
           close(p[1]);
           pid_t w2 = waitpid(pid2, &status, WUNTRACED | WCONTINUED);
           if (debugLevel >= 1) cout << "Go back to parent" << endl;
       }

      if (ShellCommand->shellStatus == Command::Go)
      {
         ShellCommand->shellStatus = ExecuteCommand();
      }

      if (debugLevel >= 2)
      {
         char temp;
         cout << "Press <ENTER> to continue...";
         temp = getchar();
      }

      if (ShellCommand->shellStatus == Command::Pause)
      {
         string unpause;
         getline(cin, unpause);
         ShellCommand->shellStatus = Command::Go;
         continue;
      }
      else if (ShellCommand->shellStatus == Command::Exit)
      {
         break;
      }
   }
}

void SimpleShell::SetFileInputMode(bool fim, string input)
{
   this->fileInputMode = fim;
   if (debugLevel >= 1) cout << "Opening file " << input << endl;
   inputFile.open(input.c_str());
   if (!inputFile.is_open())
   {
      cout << "Error opening input file, reverting to stdin" << endl;
      fileInputMode = false;
   }
}


void SimpleShell::SetDebugLevel(int dl)
{
   this->debugLevel = dl;
   cout << "Debug level set to " << dl << endl;
}


int SimpleShell::ParseInputLine()
{
   string input = "";
   size_t delimiter = 0;

   if (!fileInputMode)
   {
      cout << "sish >> ";
      getline(cin, input);
   }
   else
   {
      char fileInput[1024];
      bool readSuccess;
      do
      {
         if (debugLevel >= 1) cout << "Trying to read line" << endl;
         readSuccess = inputFile.getline(fileInput, 1024);
         if (debugLevel >= 1) cout << "Read line \"" << fileInput << "\"" << endl;
      } while (!strcmp(fileInput, "") && readSuccess);

      input = fileInput;
      if (inputFile.eof())
      {
         if (debugLevel >= 1) cout << "Reached end of file" << endl;
         ShellCommand->shellStatus = Command::Exit;
      }
   }
    while (delimiter != string::npos) //replaces tabs with spaces
    {
        delimiter = input.find('\t', delimiter);
        if (delimiter != string::npos)
        {
            input.replace(delimiter,1," ");
            delimiter++;
        }
    }
    delimiter = 0;

    while (delimiter != string::npos) //erase extra spaces
    {
        delimiter = input.find(' ', delimiter);
        if((delimiter != string::npos) && (delimiter < input.size()))
        {
            if (input[++delimiter] == ' ') input.erase(delimiter,1);
        }
    }

    delimiter = 0;

   if (ShellCommandHistory->HistorySize() >= 100) ShellCommandHistory->TrimOld(); //limit history deque size to 100
   ShellCommandHistory->PushNew(input);

   if(input.find('#') != string::npos)
   {
      input.erase(input.find('#'), string::npos);
   }


   while (delimiter != string::npos)
   {
      delimiter = input.find(' ');
      if (delimiter != string::npos)
      {
         argVector.push_back(input.substr(0, delimiter));
         input.erase(0, delimiter + 1);
      }
      else argVector.push_back(input.substr(0, string::npos));
   }
   VariableSub();
   return 0;
}

int SimpleShell::ParseInputLine(std::string repeated)
{
    argVector.clear();
   size_t delimiter = 0;


   while (delimiter != string::npos)
   {
      delimiter = repeated.find(' ');
      if (delimiter != string::npos)
      {
         argVector.push_back(repeated.substr(0, delimiter));
         repeated.erase(0, delimiter + 1);
      }
      else argVector.push_back(repeated.substr(0, string::npos));
   }
   VariableSub();
   return 0;
}

// subs in variable values before commands are exectuted - Called by ParseInputLine()
// will erase vector value if found that user is trying to set a special variable to something else
// need to work on ! and ? - values from foreground command and background command.
// may want to look into implications with export/unexport?
// echo will not be effected - is this right? i think so
// History deque will not be effected
void SimpleShell::VariableSub()
{
   size_t variablePlace = 0;
   string varString;
   ostringstream shellPid;
   ostringstream foregroundVal;
   ostringstream backgroundPid;

   shellPid << getppid(); //should it be getpid??

   if (argVector[0] != "echo") //  ignore this for echo
   {
      for (unsigned int i = 0; i < argVector.size(); i++)
      {
         variablePlace = argVector[i].find('$');
         if (variablePlace != string::npos)
         {
            varString = argVector[i].substr(variablePlace + 1);//variable value

            //check for shell specials first, if not found then replace with whats found in locals
            switch (argVector[i][variablePlace + 1])
            {
            case '$':
               if ((argVector[0] != "set") || (i == 2)) argVector[i] = shellPid.str();				// getpid(); getppid();?  //replaces with PID if not trying to set $ to local variables
               else
               {
                  argVector.erase(argVector.begin() + i);								//erase value that user is trying to set, so set will fail
                  cout << "\n Setting Shell Variables $, ?, or ! is not allowed!\n\n";
               }
               break;
            case '?':
               if ((argVector[i] != "set") || (i == 2)) argVector[i] = Command::localVariable["?"];			// hmm need to work on foreground stuff
               else
               {
                  argVector.erase(argVector.begin() + i);
                  cout << "\n Setting Shell Variables $, ?, or ! is not allowed!\n\n";
               }
               break;
            case '!':
               if ((argVector[0] != "set") || (i == 2)) argVector[i] = Command::localVariable["!"];			//hmmx2 need to work on background stuff
               else
               {
                  argVector.erase(argVector.begin() + i);
                  cout << "\n Setting Shell Variables $, ?, or ! is not allowed!\n\n";
               }
               break;
            default:
               if ((ShellCommand->localVariable.find(varString)) != (Command::localVariable.end()))		// search for variable name in locals
               {
                  argVector[i].replace(variablePlace, string::npos, Command::localVariable[varString]);	//replace $variable in argVector with localVar value
               }
               else
               {
                  cout << "\nNo Local Variable -" << varString << "- exists\n"; //couldnt find anything
               }
            }
         }
      }
   }
   return;
}

void SimpleShell::InitEnvironment()
{
   char * homeDir = getenv("PWD");
   char * pathEnv = getenv("PATH");
   string toMap(homeDir);
   string pathMap(pathEnv);
   toMap += "/sish";
   ShellCommand->environment["shell"] = toMap;
   ShellCommand->environment["PATH"] = pathMap;
   ShellCommand->environment["parent"] = toMap;
   Command::localVariable["foregroundPIDval"] = "0";
   setenv("parent", toMap.c_str(), 1);

   return;
}

Command::ShellStates SimpleShell::ExecuteCommand()
{
   if (debugLevel >= 2) cout << "Executing " << argVector[0] << " command" << endl;

   unsigned int toInt=0;
   if (argVector[0] == "show")          ((CommandSHOW*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "set")      ((CommandSET*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "unset")    ((CommandUNSET*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "export")   ((CommandEXPORT*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "unexport") ((CommandUNEXPORT*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "environ")  ((CommandENVIRON*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "chdir")    ((CommandCHDIR*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "exit")
   {
      ((CommandEXIT*)ShellCommand)->Execute(argVector);
      return Command::Exit;
   }
   else if (argVector[0] == "wait")     ((CommandWAIT*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "clr")      ((CommandCLR*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "dir")      ((CommandDIR*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "echo")     ((CommandECHO*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "help")     ((CommandHELP*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "pause")    return ((CommandPAUSE*)ShellCommand)->Execute(argVector);
   else if (argVector[0] == "history")  ((CommandHISTORY*)ShellCommand)->Execute(argVector, ShellCommandHistory);
   else if (argVector[0] == "repeat")
   {
        fflush(stdin);
        if(argVector.size() == 1)
        {
            cout << '\n' << ShellCommandHistory->Get(ShellCommandHistory->HistorySize() - 2) << '\n';
            ParseInputLine(ShellCommandHistory->Get(ShellCommandHistory->HistorySize() - 2));
            if (argVector[0] == "repeat")
            {
                cout << '\n' << "Please Dont Repeat a Repeat Command, way too recursive for me" << '\n';
                return Command::Go;
            }
            CheckPiped();
            ShellCommand->shellStatus = ExecuteCommand();

        }
        else if(argVector.size() == 2)
        {
            toInt = atoi(argVector[1].c_str());
            cout << '\n' << ShellCommandHistory->Get(toInt - 1) << '\n';
            if((toInt > 0)&&(toInt <= ShellCommandHistory->HistorySize()))
            {
                ParseInputLine(ShellCommandHistory->Get(toInt - 1));
                if (argVector[0] == "repeat")
                {
                    cout << '\n' << "Please Dont Repeat a Repeat Command, way too recursive for me" << '\n';
                    return Command::Go;
                }
                CheckPiped();
                ShellCommand->shellStatus = ExecuteCommand();
            }
        }
   }
   else if (argVector[0] == "kill")     ((CommandKILL*)ShellCommand)->Execute(argVector);
   else
   {
      if (debugLevel >= 1) cout << "EXTERNAL COMMAND" << endl;
      ((CommandEXTERNAL*)ShellCommand)->Execute(argVector);
   }

   return Command::Go;
}

void SimpleShell::InitSigHandler()
{

   signal(SIGINT, HandleSIGNAL);
   signal(SIGQUIT, HandleSIGNAL);
   signal(SIGCONT, HandleSIGNAL);
   signal(SIGSTOP, HandleSIGNAL);
   signal(SIGALRM, HandleSIGNAL);
   signal(SIGABRT, HandleSIGNAL);
   signal(SIGHUP, HandleSIGNAL);
   signal(SIGTERM, HandleSIGNAL);
   signal(SIGUSR1, HandleSIGNAL);
   signal(SIGUSR2, HandleSIGNAL);
}
void SimpleShell::HandleSIGNAL(int sig)
{
//   std::cout << "\n\n Signal Caught\n\n";
//   std::cout << "\nsish >>";
   fflush(stdout);
   int status;
   int pid = atoi(Command::localVariable["foregroundPIDval"].c_str());
   if (sig == SIGINT || SIGQUIT || SIGCONT || SIGSTOP) // only need to pass these signals to foreground processes
   {
        if(pid>0) kill(pid, sig);//pass on the caught signal to all foreground
        waitpid(pid,&status,0);
   }
}

/*
-    Check Piped will look for '|' as a delimiter
-    each command and all its arguments will exist in the same element - space delimited
-    returns true if it found a pipe
-*/
bool SimpleShell::CheckPiped()
{
   unsigned int pipePlace = 0;
   isPiped = false;
   pipedVector.clear();
   if (argVector.size())
   {
      pipedVector.push_back("");
      for (unsigned int i = 0; i < argVector.size(); i++)
      {
         if (argVector[i].find('|') != string::npos)
         {
            isPiped = true;
            pipePlace++;
            pipedVector.push_back("");
         }
         else
         {
            if (pipedVector[pipePlace] != "") pipedVector[pipePlace].append(" ");
            pipedVector[pipePlace].append(argVector[i]);
         }

      }
   }

   return isPiped;
}

