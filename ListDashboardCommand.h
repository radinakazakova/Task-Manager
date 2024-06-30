#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class ListDashboardCommand : public AuthCommand {
public:
	ListDashboardCommand(Session& context);
protected:
	void executeWithUser(User* user) override;
};
