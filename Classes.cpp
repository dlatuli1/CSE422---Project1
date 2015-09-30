
#include "Classes.h"
std::deque<std::string> ShellCommandHistory::commandHistory;

void ShellCommandHistory::PushNew(std::string command)
{
	commandHistory.push_back(command);
	return;
}
void ShellCommandHistory::TrimOld()
{
	commandHistory.erase(commandHistory.begin());
	return;
}
unsigned int ShellCommandHistory::HistorySize()
{
	return  commandHistory.size();
}
std::string ShellCommandHistory::Get(size_t location)
{
	return commandHistory[location];
}
