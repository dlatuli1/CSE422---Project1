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
   //Execute show command
   for (unsigned int i = 1; i < argVector.size(); i++)
   {
      cout << argVector[i] << endl;
   }
   return 0;
}

int CommandSET::Execute(vector<string> argVector)
{
   //Execute set command
   if (argVector.size() == 3)
   {
      Command::localVariable[argVector[1]] = argVector[2]; // push to shell var map
   }
   else
   {
      cout << "Usage: set W1 W2" << endl;
   }
   return 0;
}

int CommandUNSET::Execute(vector<string> argVector)
{
   //Execute unset command
   if (argVector.size() == 2)
   {
      Command::localVariable.erase(argVector[1]); //remove from shell var map
   }
   else
   {
      cout << "Usage: unset W" << endl;
   }
   return 0;
}

int CommandEXPORT::Execute(vector<string> argVector)
{
   //Execute export command
   if (argVector.size() == 3)
   {
      // since setenv() takes char* type, we have to first convert the arguments into char*
      char *cStringArg1 = new char[argVector[1].length() + 1];
      char *cStringArg2 = new char[argVector[2].length() + 1];
      strcpy(cStringArg1, argVector[1].c_str()); //variable name
      strcpy(cStringArg2, argVector[2].c_str()); //variable value
      environment[argVector[1]] = argVector[2]; //push onto shell env map
      setenv(cStringArg1, cStringArg2, true); //push to env
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
   //Execute unexport command
   if (argVector.size() == 2)
   {
      // since unsetenv() takes char* type, we have to first convert the arguments into char*
      char *cStringArg = new char[argVector[1].length() + 1];
      strcpy(cStringArg, argVector[1].c_str());
      unsetenv(cStringArg);
      delete cStringArg;
      environment.erase(argVector[1]); //remove it from shell environment map as well
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
    // loop thru and print all the shell environment variables
   for (map<string, string>::iterator it = environment.begin(); it != environment.end(); it++)
   {
      string envString = it->first + "=" + it->second;
      cout << envString << '\n';
   }
   return 0;
}

int CommandCHDIR::Execute(vector<string> argVector)
{
     if(argVector.size() == 2)
    {
        string pathString = environment["CWD"]; //get the shell environments working path
        if (argVector[1].find("..") != string::npos) // if '..' :move down directory
        {
            size_t parentDir = pathString.find_last_of('/', string::npos); // position of last /
            pathString.erase(parentDir,string::npos); // erase everything after
        }
        else if (argVector[1].find(".") != string::npos)
        {

            pathString = pathString + argVector[1].substr(argVector[1].find_last_of("/")); //relative path + new arg
        }
        else
        {
            pathString = argVector[1]; //absolute path
        }
        environment["CWD"] = pathString; //reset with new dir
    }
   return 0;
}

int CommandEXIT::Execute(vector<string> argVector)
{
   //Execute exit command

   return atoi(argVector[1].c_str());
}

int CommandWAIT::Execute(vector<string> argVector)
{
    int status;
    if (argVector.size() == 2)
    {
        pid_t pid = atoi(argVector[1].c_str()); //convert arg to int
        if(pid > 0) waitpid(pid,&status,0); // wait on pid
        else waitpid(0,&status,0); // wait on all pid with parentspid
    }
   return 0;
}


int CommandCLR::Execute(vector<string> argVector)
{
   //Execute clr command

   //There's no perfect way to do this that is portable
   //Temp answer:
   cout << string(100, '\n');
   return 0;
}

int CommandDIR::Execute(vector<string> argVector)
{
   cout << "Executing dir command" << endl;
   //Execute dir command
   //char workingDir_Cstr[FILENAME_MAX];
   vector<string> dirContents;
   //GetWorkingDir(workingDir_Cstr, sizeof(workingDir_Cstr));
   //string workingDir(workingDir_Cstr);
   string workingDir = environment["CWD"]; // get the shell directory variable

   cout << workingDir << '\n';

   DIR *dp;
   struct dirent *dirp;
   string DIRstr;

   if ((dp = opendir(workingDir.c_str())) == NULL) //attempt to open dir
   {
      cout << "\n Failure accessing directory\n";
      return 0;
   }
   while ((dirp = readdir(dp)) != NULL) //loop thru dir files
   {
      if (dirp->d_type == DT_DIR)
      {
         DIRstr = dirp->d_name;
         if ((DIRstr != ".") && (DIRstr != "..")) cout << "\t <DIR> " << dirp->d_name << '\n'; //show that its a directory
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
    pid_t pid;
    int status;
    // child loop after forking
    if ((pid = fork()) == 0)
    {
        fflush(stdout);
        execlp("vi", "vi", "SimpleManPage",0);  // executes vi to read the man page text file
        perror("exec error");
        exit(1);

    }
    else// for parent loop
    {
        waitpid(pid, &status, 0);//wait for user to exit vi
    }

   return 0;
}
/*
int CommandREPEAT::Execute(vector<string> argVector)
{


}
*/
Command::ShellStates CommandPAUSE::Execute(vector<string> argVector)
{
   //Execute pause command

   cout << "\n------------------------------\n\tsish paused:\n\tPress Enter to resume\n------------------------------\n";

   //either put the waiting loop here,
   //or expand on the returns for Shell operation functions like pause and exit

   return Pause;
}

int CommandHISTORY::Execute(vector<string> argVector, History* ShellCommandHistory)
{
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
      cout << '\t' << ++count << "  " << ShellCommandHistory->Get(i) << '\n'; //loop thru and cout
   }
   return 0;
}

int CommandKILL::Execute(vector<string> argVector)
{
    int pid =0;
    int sig =0;
    int status;
    if(argVector.size() == 2) // default to SIGTERM
    {
        pid = atoi(argVector[1].c_str()); //convert to pid_t
        kill(pid,SIGTERM); //pass to pid
        waitpid(pid,&status, 0); //wait for it to be killed
    }
    else if(argVector.size()==3) // signal arg recieved
    {
        pid = atoi(argVector[2].c_str()); //convert signal to int
        sig = abs (atoi(argVector[1].c_str())); // absolute value of signal (ex.'-9')
        kill(pid,sig); //pass to pid
        waitpid(pid,&status, 0); //wait for it to be killed
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
   if (argVector[0].find("..") != string::npos) //go back directories for each set of '..'s
   {
        pathString = string(workingDir_Cstr); //current dir
        while(delimiter != string::npos) //keep searching for '..'
        {
          delimiter = argVector[0].find("..",++delimiter);

          size_t parentDir = pathString.find_last_of('/', string::npos); //location of last '/'
          pathString.erase(parentDir,string::npos); //erase everything past last '/'
          if (multiParent) //if more than 1 '..' found
          {
            size_t parentDir = pathString.find_last_of('/', string::npos);//erase another dir level
            pathString.erase(parentDir,string::npos);
          }
          size_t comName = argVector[0].find_last_of('/', string::npos); //only the command name
          pathString.append(argVector[0].substr(comName, string::npos)); //append the command name to the resized path
          multiParent = true; //for the next loop thru
        }
        delimiter = 0;
      //cout << pathString << '\n';
   }
       else if (argVector[0].find(".") != string::npos) //relative path '.'
       {
          pathString = string(workingDir_Cstr); //current dir
          //pathString += "/";
          size_t comName = argVector[0].find_last_of('.', string::npos) + 1; // command name only
          pathString.append(argVector[0], comName, string::npos); // append command name to current path
        //  cout << pathString << '\n';
       }
       else
       {
          pathString = argVector[0]; //absolute path only, or search thru PATH variable
       }

   // Since the execve() function takes char*, argVector has to be modified. vc is the modified char** equivalent of argVector
   // char *convert(const string & s) defined in SupportingFunctions.h
   transform(argVector.begin(), argVector.end(), back_inserter(vc), convert);
   vc.push_back(NULL); // exec needs null terminated

   if ((pid = fork()) == 0) {      //Child process

      fflush(stdout);

      if (execvp(pathString.c_str(), &vc[0]))
      {
         perror("child process execve failed [%m]");
         exit(1);
      }
   }
   else{ //Parent process
      // fflush(stdout);
//      cout << "Parent process waiting..." << endl;
      if (!background) //foreground command
      {
         Command::localVariable["foregroundPIDval"] = to_string(pid); //push the running pid to variable
         waitpid(pid, &status, 0); //wait for foreground command to finish
         Command::localVariable["?"] = to_string(status); //push the return value to ? variable
         if (status != 0){
            cerr << "Child process end error!" << endl;
            kill(pid, SIGKILL);
         }
      }
      else //background command, dont wait
      {
         Command::localVariable["!"] = to_string(pid); //push running pid to ! variable
      }
   }


   for (size_t i = 0; i < vc.size(); i++)
      delete[] vc[i];

   return 0;
}

