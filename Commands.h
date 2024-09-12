#pragma once
#include "MyString.h"
#include "System.h"

class Command
{
public:
	Command(System& system);
	virtual void execute() = 0;
	virtual ~Command() = default;
protected:
	System& system;
};

class RegisterCommand : public Command
{
public:
	RegisterCommand(System& system, const MyString& username, const MyString& password);
	void execute() override;

private:
	MyString username;
	MyString password;
};

class LoginCommand : public Command
{
public:
	LoginCommand( System& system, const MyString& username, const MyString& password);
	void execute() override;
private:
	MyString username;
	MyString password;
};

class AddTaskCommand : public Command
{
public:
	AddTaskCommand( System& system, const MyString& name, const std::tm& due_date, const MyString& description);
	AddTaskCommand( System& system, const MyString& name, const MyString& description);

	void execute() override;
private:
	MyString name;
	std::tm due_date;
	MyString description;
	bool hasDueDate = false;
};

class UpdateTaskNameCommand : public Command
{
public:
	UpdateTaskNameCommand( System& system, int id, const MyString& name);
	void execute() override;
private:
	int id;
	MyString name;
};

class StartTaskCommand : public Command
{
public:
	StartTaskCommand( System& system, int id);
	void execute() override;
private:
	int id;

};

class UpdateTaskDescriptionCommand : public Command
{
public:
	UpdateTaskDescriptionCommand( System& system, int id, const MyString& desc);
	void execute() override;
private:
	int id;
	MyString desc;
};

class RemoveTaskFromDashboardCommand : public Command
{
public:
	RemoveTaskFromDashboardCommand( System& system, int id);
	void execute() override;
private:
	int id;
};

class AddTaskToDashboardCommand : public Command
{
public:
	AddTaskToDashboardCommand( System& system, int id);
	void execute() override;
private:
	int id;

};

class DeleteTaskCommand : public Command
{
public:
	DeleteTaskCommand( System& system, int id);
	void execute() override;
private:
	int id;
};

class GetTaskCommand : public Command
{
public:
	GetTaskCommand( System& system, int id);
	GetTaskCommand( System& system, const MyString& name);
	void execute() override;
private:
	int id = -1;
	MyString name;
};

class ListTasksCommand : public Command
{
public:
	ListTasksCommand( System& system, const std::tm& date);
	ListTasksCommand( System& system );
	ListTasksCommand( System& system, const MyString& collabName);
	void execute() override;

private:
	std::tm date;
	bool hasDate = false;
	MyString collabName;
	bool hasCollabName = false;
};

class ListCompletedTasksCommand : public Command
{
public:
	ListCompletedTasksCommand( System& system);
	void execute() override;

};

class ListDashboardCommand : public Command
{
public:
	ListDashboardCommand( System& system);
	void execute() override;

};

class FinishTaskCommand : public Command
{
public:
	FinishTaskCommand( System& system, int id);
	void execute() override;
private:
	int id;
};

class LogoutCommand : public Command
{
public:
	LogoutCommand( System& system);
	void execute() override;

};

class AddCollaborationCommand : public Command
{
public:
	AddCollaborationCommand( System& system, const MyString& name);
	void execute() override;
private:
	MyString name;
};

class DeleteCollaborationCommand : public Command
{
public:
	DeleteCollaborationCommand( System& system, const MyString& name);
	void execute() override;
private:
	MyString name;
};

class listCollaborationsCommand : public Command
{
public:
	listCollaborationsCommand( System& system);
	void execute() override;

};

class AddUserCommand : public Command
{
public:
	AddUserCommand( System& system, const MyString& collaborationName, const MyString& username);
	void execute() override;
private:
	MyString collaborationName;
	MyString username;
};

class AssignTaskCommand : public Command
{
public:

	AssignTaskCommand(System& system, const MyString& collaborationName, const MyString& username,
	                  const MyString& taskName, const std::tm& due_date, const MyString& desc);
	AssignTaskCommand(System& system, const MyString& collaborationName, const MyString& username, const MyString& taskName, const MyString& desc);

	void execute() override;
private:
	MyString collaborationName;
	MyString username;
	MyString taskName;
	std::tm due_date;
	MyString desc;

	bool hasDate = false;
};
