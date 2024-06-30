#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include <ctime>

class AddTaskCommand : public AuthCommand {
public:
	AddTaskCommand(Session& context, const MyString& name, const std::tm& due_date, const MyString& description);
	AddTaskCommand(Session& context, const MyString& name, const MyString& description);
protected:
	void executeWithUser(User* user) override;
private:
	MyString name, description;
	std::tm due_date;
	bool hasDueDate = false;
};