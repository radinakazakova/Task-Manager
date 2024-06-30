#pragma once
#include "Command.h"
#include "MyString.h"

class LoginCommand : public Command {
public:
	LoginCommand(Session& context, const MyString& username, const MyString& password);
	void execute() override;
private:
	MyString username, password;
};