#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class DeleteTaskCommand : public AuthCommand {
public:
	DeleteTaskCommand(Session& context, const int id);
protected:
	void executeWithUser(User* user) override;
private:
	int id;
};
