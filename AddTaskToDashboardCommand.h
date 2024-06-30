#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class AddTaskToDashboardCommand : public AuthCommand {
public:
	AddTaskToDashboardCommand(Session& context, const int id);
protected:
	void executeWithUser(User* user) override;
private:
	int id;
};
