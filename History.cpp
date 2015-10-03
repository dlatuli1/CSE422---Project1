#include "History.h"

History::History()
{
}

History::~History()
{
}

unsigned int History::HistorySize()
{
	return commandHistory.size();
}

void History::TrimOld()
{
	commandHistory.erase(commandHistory.begin());
	return;
}

void History::PushNew(string command)
{
	commandHistory.push_back(command);
	return;
}

string History::Get(size_t location)
{
	return commandHistory[location];
}
