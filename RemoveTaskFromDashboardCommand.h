#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class RemoveTaskFromDashboardCommand : public AuthCommand {
public:
	RemoveTaskFromDashboardCommand(Session& context, const int id);
protected:
	void executeWithUser(User* user) override;
private:
	int id;
};
