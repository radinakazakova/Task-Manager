#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class AddUserCommand : public AuthCommand {
public:
	AddUserCommand(Session& context, const MyString& collaborationName, const MyString& username);
protected:
	void executeWithUser(User* user) override;
private:
	MyString collaborationName, username;
};
