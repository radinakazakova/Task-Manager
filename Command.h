#pragma once
#include "Session.h"

class Command {
public:
	Command(Session& context);
	virtual void execute() = 0;
	virtual ~Command() = default;
protected:
	Session& context;
};