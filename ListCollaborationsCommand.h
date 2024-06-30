#pragma once
#include "AuthCommand.h"
#include "Session.h"
#include "User.h"

class ListCollaborationsCommand : public AuthCommand {
public:
	ListCollaborationsCommand(Session& context);
protected:
	void executeWithUser(User* user) override;
};
