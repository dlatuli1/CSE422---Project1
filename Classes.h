#pragma once
#include <deque>
#include <string>


class ShellCommandHistory
{
	static std::deque<std::string> commandHistory;
public:
	static void PushNew(std::string command);
	static void TrimOld();
	static unsigned int HistorySize();
	static std::string Get(size_t location);
};


