#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class UpdateTaskNameCommand : public AuthCommand {
public:
	UpdateTaskNameCommand(Session& context, const int id, const MyString& name);
protected:
	void executeWithUser(User* user) override;
private:
	int id;
	MyString name;
};
