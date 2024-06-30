#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class ListCompletedTasksCommand : public AuthCommand {
public:
	ListCompletedTasksCommand(Session& context);
protected:
	void executeWithUser(User* user) override;
};
