#pragma once
#include "Command.h"
#include "Commands.h"
#include "User.h"

class AuthCommand : public Command {
public:
	AuthCommand(Session& context);
	void execute() override final;
protected:
	virtual void executeWithUser(User* user) = 0;
};
