#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class DeleteCollaborationCommand : public AuthCommand {
public:
	DeleteCollaborationCommand(Session& context, const MyString& name);
protected:
	void executeWithUser(User* user) override;
private:
	MyString name;
};
