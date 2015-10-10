#include "Commands.h"
#include "SupportingFunctions.h"

Command::Command()
{
}

Command::~Command()
{
}

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
      localVariable[argVector[1]] = argVector[2];
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
      localVariable.erase(argVector[1]);
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
   /*
   extern char **environ;
   //char *s = *environ;
   for(int i = 1; environ[i]!=NULL; i++)
   cout << environ[i] << '\n';
   */
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
   cout << "Executing wait command" << endl;
   //Execute wait command

   //Can't do much until children are created...
   //Temp answer:
   //
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

int CommandEXTERNAL::Execute(vector<string> argVector)
{
   //const string searchPath = "/bin/";//getenv("PATH");
   //string externalCommand = searchPath + argVector[0];
   string pathString;
   pid_t pid;
   int status;//, timeout;//, counter;
   bool background = false;
   vector<char*> vc;
   char workingDir_Cstr[FILENAME_MAX];
   GetWorkingDir(workingDir_Cstr, sizeof(workingDir_Cstr));
   //   cout << '\n' << argVector[0];
   //    char * passEviron[environment.size()+1];
   //    extern char ** environ;

   if (argVector.back() == "!") //background command
   {
      background = true;
      argVector.pop_back();
   }

   if (argVector[0].find("..") != string::npos)
   {
      pathString = string(workingDir_Cstr);
      size_t parentDir = pathString.find_last_of('/', string::npos) + 1;
      size_t comName = argVector[0].find_last_of('.', string::npos) + 1;
      pathString.replace(parentDir, string::npos, argVector[0].substr(comName, string::npos));
      //cout << pathString << '\n';
   }
   else if (argVector[0].find(".") != string::npos)
   {
      pathString = string(workingDir_Cstr);
      pathString += "/";
      size_t comName = argVector[0].find_last_of('.', string::npos) + 1;
      pathString.append(argVector[0], comName, string::npos);
      //cout << pathString << '\n';
   }
   else
   {
      pathString = argVector[0];
   }

   // Since the execve() function takes char*, argVector has to be modified. vc is the modified char** equivalent of argVector
   // char *convert(const string & s) defined in SupportingFunctions.h
   transform(argVector.begin(), argVector.end(), back_inserter(vc), convert);
   vc.push_back(NULL);
   /*
   for (vector<char*>::iterator it = vc.begin(); it != vc.end(); it++)
   {
   cout << '\n' << *it << ' ';
   }*/
   //    counter = 0;
   /*    for (map<string, string>::iterator it =  environment.begin(); it != environment.end(); it++)
   {
   envString = it->first + "=" + it->second;
   char * copystr = new char [envString.length() + 1];
   strcpy(copystr, envString.c_str());
   passEviron[counter++] = copystr;
   cout << "\n\n" << passEviron[counter-1] << "\n\n";
   delete[] copystr;
   }*/

   if ((pid = fork()) == 0) {
      //Child process
      fflush(stdout);
      //        char **childEnviron = environ;
      //       if (execve(externalCommand.c_str(), &vc[0] , NULL) == -1)
      if (execvp(pathString.c_str(), &vc[0]))
      {
         perror("child process execve failed [%m]");
         //           cout << argVector[0] << endl;
         //           environ = childEnviron;
         exit(1);
      }
   }
   else{ //Parent process
      // fflush(stdout);
      cout << "Parent process waiting..." << endl;
      //       cout << "\nparent Looking at:" << pid << '\n';
      if (!background)
      {
         waitpid(pid, &status, 0);
         localVariable["?"] = to_string(status);
         if (status != 0){
            cerr << "Child process end error!" << endl;
            kill(pid, SIGKILL);
            //return -1;
         }
      }
      else
      {
         //           cout << "setting var\n";
         localVariable["!"] = to_string(pid);
         //           cout << localVariable["!"];
      }
      //       kill(pid, SIGTERM);
   }

   /*#ifdef WAIT_FOR_COMPLETION
   timeout = 1000;
   while (waitpid(pid , &status , WNOHANG) == 0) {
   if ( --timeout < 0 ) {
   perror("timeout");
   return -1;
   }
   sleep(1);
   }
   cout << argVector[0] << " WEXITSTATUS " <<  WEXITSTATUS(status) << " WIFEXITED" << WIFEXITED(status) << " [status " << status << "]" << endl;

   if (WIFEXITED(status) != 1 || WEXITSTATUS(status) != 0) {
   perror("%s failed, halt system");
   return -1;
   }
   #endif*/

   for (size_t i = 0; i < vc.size(); i++)
      delete[] vc[i];

   return 0;
}
