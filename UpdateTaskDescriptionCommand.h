#pragma once
#include "AuthCommand.h"
#include "MyString.h"

class UpdateTaskDescriptionCommand : public AuthCommand
{
private:
	int id;
	MyString description;

public:
	UpdateTaskDescriptionCommand(Session& context, int id, const MyString& description);

	void executeWithUser(User* user) override;
};