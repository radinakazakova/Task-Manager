#pragma once
#include "AuthCommand.h"
#include "MyString.h"
#include "Session.h"
#include "User.h"

class ListTasksCommand : public AuthCommand {
public:
	ListTasksCommand(Session& context, const std::tm& date);
	ListTasksCommand(Session& context);
	ListTasksCommand(Session& context, const MyString& collaborationName);
protected:
	void executeWithUser(User* user) override;
private:
	std::tm date;
	MyString collaborationName;
	bool hasDate = false;
	bool hasCollabName = false;
};
