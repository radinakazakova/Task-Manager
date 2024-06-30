#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class AddCollaborationCommand : public AuthCommand {
public:
	AddCollaborationCommand(Session& context, const MyString& name);
protected:
	void executeWithUser(User* user) override;
private:
	MyString name;
};
