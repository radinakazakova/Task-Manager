#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class StartTaskCommand : public AuthCommand {
public:
	StartTaskCommand(Session& context, const int id);
protected:
	void executeWithUser(User* user) override;
private:
	int id;
};
