#pragma once
#include "Session.h"
#include "AllCommands.h"
#include "MyString.h"
#include <sstream>

class CommandParser
{
public:
	CommandParser() = delete;

	static Command* parse(Session& context, const MyString& commandName, std::stringstream& ss);
};