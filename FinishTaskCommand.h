#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class FinishTaskCommand : public AuthCommand {
public:
	FinishTaskCommand(Session& context, const int id);
protected:
	void executeWithUser(User* user) override;
private:
	int id;
};
