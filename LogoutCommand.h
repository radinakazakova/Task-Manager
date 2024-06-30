#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class LogoutCommand : public AuthCommand {
public:
	LogoutCommand(Session& context);
protected:
	void executeWithUser(User* user) override;
};
