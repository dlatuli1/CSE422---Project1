#include "Commands.h"
#include "SupportingFunctions.h"

Command::Command()
{
}

Command::~Command()
{
}

map<string,string> Command::localVariable;

int Command::Execute()
{
   return 0;
}

int CommandSHOW::Execute(vector<string> argVector)
{
   cout << "Executing show command" << endl;
   //Execute show command
   for (unsigned int i = 1; i < argVector.size(); i++)
   {
      cout << argVector[i] << endl;
   }
   return 0;
}

int CommandSET::Execute(vector<string> argVector)
{
   cout << "Executing set command" << endl;
   //Execute set command
   if (argVector.size() == 3)
   {
      Command::localVariable[argVector[1]] = argVector[2];
   }
   else
   {
      cout << "Usage: set W1 W2" << endl;
   }
   return 0;
}

int CommandUNSET::Execute(vector<string> argVector)
{
   cout << "Executing unset command" << endl;
   //Execute unset command
   if (argVector.size() == 2)
   {
      Command::localVariable.erase(argVector[1]);
   }
   else
   {
      cout << "Usage: unset W" << endl;
   }
   return 0;
}

int CommandEXPORT::Execute(vector<string> argVector)
{
   cout << "Executing export command" << endl;
   //Execute export command
   if (argVector.size() == 3)
   {
      // since setenv() takes char* type, we have to first convert the arguments into char*
      char *cStringArg1 = new char[argVector[1].length() + 1];
      char *cStringArg2 = new char[argVector[2].length() + 1];
      strcpy(cStringArg1, argVector[1].c_str());
      strcpy(cStringArg2, argVector[2].c_str());
      environment[argVector[1]] = argVector[2];
      setenv(cStringArg1, cStringArg2, true);
      delete cStringArg1;
      delete cStringArg2;
   }
   else
   {
      cout << "Usage: export W1 W2" << endl;
   }
   return 0;
}

int CommandUNEXPORT::Execute(vector<string> argVector)
{
   cout << "Executing unexport command" << endl;
   //Execute unexport command
   if (argVector.size() == 2)
   {
      // since unsetenv() takes char* type, we have to first convert the arguments into char*
      char *cStringArg = new char[argVector[1].length() + 1];
      strcpy(cStringArg, argVector[1].c_str());
      unsetenv(cStringArg);
      delete cStringArg;
      environment.erase(argVector[1]);
   }
   else
   {
      cout << "Usage: unexport W" << endl;
   }
   return 0;
}

int CommandENVIRON::Execute(vector<string> argVector)
{
   cout << endl;
   //Execute environ command

   for (map<string, string>::iterator it = environment.begin(); it != environment.end(); it++)
   {
      string envString = it->first + "=" + it->second;
      cout << envString << '\n';
   }
   return 0;
}

int CommandCHDIR::Execute(vector<string> argVector)
{
   cout << "Executing chdir command" << endl;
   //Execute chdir command
   return 0;
}

int CommandEXIT::Execute(vector<string> argVector)
{
   cout << "Executing exit command" << endl;
   //Execute exit command

   return atoi(argVector[1].c_str());
}

int CommandWAIT::Execute(vector<string> argVector)
{
    int status;
    if (argVector.size() == 2)
    {
        cout << "Executing wait command" << endl;
        pid_t pid = stoi(argVector[1]);
        if(pid > 0) waitpid(pid,&status,0);
        else waitpid(0,&status,0);
    }
   return 0;
}


int CommandCLR::Execute(vector<string> argVector)
{
   cout << "Executing clr command" << endl;
   //Execute clr command

   //There's no perfect way to do this that is portable
   //Temp answer:
   //cout << string(100, '\n');
   return 0;
}

int CommandDIR::Execute(vector<string> argVector)
{
   cout << "Executing dir command" << endl;
   //Execute dir command
   char workingDir_Cstr[FILENAME_MAX];
   vector<string> dirContents;
   GetWorkingDir(workingDir_Cstr, sizeof(workingDir_Cstr));
   string workingDir(workingDir_Cstr);

   cout << workingDir << '\n';

   DIR *dp;
   struct dirent *dirp;
   string DIRstr;

   if ((dp = opendir(workingDir.c_str())) == NULL)
   {
      cout << "\n Failure accessing directory\n";
      return 0;
   }
   while ((dirp = readdir(dp)) != NULL)
   {
      if (dirp->d_type == DT_DIR)
      {
         DIRstr = dirp->d_name;
         if ((DIRstr != ".") && (DIRstr != "..")) cout << "\t <DIR> " << dirp->d_name << '\n';
      }
      else
      {
         cout << "\t" << dirp->d_name << '\n';
      }
   }
   closedir(dp);

   return 0;
}

int CommandECHO::Execute(vector<string> argVector)
{
   cout << "Executing echo command" << endl;
   //Execute echo command
   for (unsigned int i = 1; i < argVector.size(); i++)
   {
      cout << argVector[i] << ' ';
   }
   cout << '\n';
   return 0;
}

int CommandHELP::Execute(vector<string> argVector)
{
   cout << "Executing help command" << endl;
   //Execute help command
   return 0;
}
/*
int CommandREPEAT::Execute(vector<string> argVector)
{


}
*/
Command::ShellStates CommandPAUSE::Execute(vector<string> argVector)
{
   cout << "Executing pause command" << endl;
   //Execute pause command

   cout << "\n------------------------------\n\tsish paused:\n\tPress Enter to resume\n------------------------------\n";

   //either put the waiting loop here,
   //or expand on the returns for Shell operation functions like pause and exit

   return Pause;
}

int CommandHISTORY::Execute(vector<string> argVector, History* ShellCommandHistory)
{
   cout << "Executing history command" << endl;
   unsigned int count = 0, history_asked = 0;
   if (argVector.size() > 1)
   {
      if (isdigit(argVector[1][0])) history_asked = atoi(argVector[1].c_str());  //converting string parameter to int
   }

   if ((history_asked >= ShellCommandHistory->HistorySize()) || (history_asked == 0))
   {
      history_asked = ShellCommandHistory->HistorySize() - 1; //trimming max history asked to current size
   }

   for (unsigned int i = (ShellCommandHistory->HistorySize() - history_asked - 1); i < ShellCommandHistory->HistorySize() - 1; i++)  // '-1's are so current history command is not printed to console;
   {
      cout << '\t' << ++count << "  " << ShellCommandHistory->Get(i) << '\n';
   }
   return 0;
}

int CommandKILL::Execute(vector<string> argVector)
{
    int pid =0;
    int sig =0;
    int status;
    if(argVector.size() == 2)
    {
        pid = stoi(argVector[1]);
        kill(pid,SIGTERM);
        waitpid(pid,&status, 0);
    }
    else if(argVector.size()==3)
    {
        pid = stoi(argVector[2]);
        sig = abs (stoi(argVector[1]));
        kill(pid,sig);
        waitpid(pid,&status, 0);
    }
return 0;
}

int CommandEXTERNAL::Execute(vector<string> argVector)
{

   string pathString;
   pid_t pid;
   size_t delimiter = 0;
   int status;
   bool multiParent = false;
   bool background = false;
   vector<char*> vc;
   char workingDir_Cstr[FILENAME_MAX];
   GetWorkingDir(workingDir_Cstr, sizeof(workingDir_Cstr));


   if (argVector.back() == "!") //background command
   {
      background = true;
      argVector.pop_back();
   }
   if (argVector[0].find("..") != string::npos)
   {
        pathString = string(workingDir_Cstr);
        while(delimiter != string::npos)
        {
          delimiter = argVector[0].find("..",++delimiter);

          size_t parentDir = pathString.find_last_of('/', string::npos);
          pathString.erase(parentDir,string::npos);
          if (multiParent)
          {
            size_t parentDir = pathString.find_last_of('/', string::npos);
            pathString.erase(parentDir,string::npos);
          }
          size_t comName = argVector[0].find_last_of('/', string::npos);
          pathString.append(argVector[0].substr(comName, string::npos));
          multiParent = true;
        }
        delimiter = 0;
      cout << pathString << '\n';
   }
       else if (argVector[0].find(".") != string::npos)
       {
          pathString = string(workingDir_Cstr);
          //pathString += "/";
          size_t comName = argVector[0].find_last_of('.', string::npos) + 1;
          pathString.append(argVector[0], comName, string::npos);
          cout << pathString << '\n';
       }
       else
       {
          pathString = argVector[0];
       }

   // Since the execve() function takes char*, argVector has to be modified. vc is the modified char** equivalent of argVector
   // char *convert(const string & s) defined in SupportingFunctions.h
   transform(argVector.begin(), argVector.end(), back_inserter(vc), convert);
   vc.push_back(NULL);

   if ((pid = fork()) == 0) {
      //Child process
      fflush(stdout);

      if (execvp(pathString.c_str(), &vc[0]))
      {
         perror("child process execve failed [%m]");
         exit(1);
      }
   }
   else{ //Parent process
      // fflush(stdout);
      cout << "Parent process waiting..." << endl;
      if (!background)
      {
         Command::localVariable["foregroundPIDval"] = to_string(pid);
         waitpid(pid, &status, 0);
         Command::localVariable["?"] = to_string(status);
         if (status != 0){
            cerr << "Child process end error!" << endl;
            kill(pid, SIGKILL);
         }
      }
      else
      {
         Command::localVariable["!"] = to_string(pid);
      }
   }


   for (size_t i = 0; i < vc.size(); i++)
      delete[] vc[i];

   return 0;
}
