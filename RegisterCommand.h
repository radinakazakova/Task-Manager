#pragma once
#include "Command.h"
#include "MyString.h"
#include "Session.h"

class RegisterCommand : public Command {
public:
	RegisterCommand(Session& context, const MyString& username, const MyString& password);
	void execute() override;
private:
	MyString username, password;
};
