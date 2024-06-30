#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class AssignTaskCommand : public AuthCommand {
public:
	AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
		const MyString& taskName, const std::tm& due_date, const MyString& desc);
	AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
		const MyString& taskName, const MyString& desc);
protected:
	void executeWithUser(User* user) override;
private:
	MyString collaborationName, username, taskName, desc;
	std::tm due_date;
	bool hasDate = false;
};
