#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class GetTaskCommand : public AuthCommand {
public:
	GetTaskCommand(Session& context, const int id);
	GetTaskCommand(Session& context, const MyString& name);
protected:
	void executeWithUser(User* user) override;
private:
	int id = -1;
	MyString name;
};
