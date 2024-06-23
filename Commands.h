#pragma once
#include "MyString.h"
#include "System.h"

class Command
{
public:
	Command(const System& system);
	virtual void execute() = 0;
	virtual ~Command() = default;
protected:
	System system;
};

class RegisterCommand : public Command
{
public:
	RegisterCommand(const System& system, const MyString& username, const MyString& password);
	void execute() override;

private:
	MyString username;
	MyString password;
};

class LoginCommand : public Command
{
public:
	LoginCommand(const System& system, const MyString& username, const MyString& password);
	void execute() override;
private:
	MyString username;
	MyString password;
};

class AddTaskCommand : public Command
{
public:
	AddTaskCommand(const System& system, const MyString& name, const std::tm& due_date, const MyString& description);
	AddTaskCommand(const System& system, const MyString& name, const MyString& description);

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
	UpdateTaskNameCommand(const System& system, int id, const MyString& name);
	void execute() override;
private:
	int id;
	MyString name;
};

class StartTaskCommand : public Command
{
public:
	StartTaskCommand(const System& system, int id);
	void execute() override;
private:
	int id;

};

class UpdateTaskDescriptionCommand : public Command
{
public:
	UpdateTaskDescriptionCommand(const System& system, int id, const MyString& desc);
	void execute() override;
private:
	int id;
	MyString desc;
};

class RemoveTaskFromDashboardCommand : public Command
{
public:
	RemoveTaskFromDashboardCommand(const System& system, int id);
	void execute() override;
private:
	int id;
};

class AddTaskToDashboardCommand : public Command
{
public:
	AddTaskToDashboardCommand(const System& system, int id);
	void execute() override;
private:
	int id;

};

class DeleteTaskCommand : public Command
{
public:
	DeleteTaskCommand(const System& system, int id);
	void execute() override;
private:
	int id;
};

class GetTaskCommand : public Command
{
public:
	GetTaskCommand(const System& system, int id);
	GetTaskCommand(const System& system, const MyString& name);
	void execute() override;
private:
	int id = -1;
	MyString name;
};

class ListTasksCommand : public Command
{
public:
	ListTasksCommand(const System& system, const std::tm& date);
	ListTasksCommand(const System& system );
	ListTasksCommand(const System& system, const MyString& collabName);
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
	ListCompletedTasksCommand(const System& system);
	void execute() override;

};

class ListDashboardCommand : public Command
{
public:
	ListDashboardCommand(const System& system);
	void execute() override;

};

class FinishTaskCommand : public Command
{
public:
	FinishTaskCommand(const System& system, int id);
	void execute() override;
private:
	int id;
};

class LogoutCommand : public Command
{
public:
	LogoutCommand(const System& system);
	void execute() override;

};

class AddCollaborationCommand : public Command
{
public:
	AddCollaborationCommand(const System& system, const MyString& name);
	void execute() override;
private:
	MyString name;
};

class DeleteCollaborationCommand : public Command
{
public:
	DeleteCollaborationCommand(const System& system, const MyString& name);
	void execute() override;
private:
	MyString name;
};

class listCollaborationsCommand : public Command
{
public:
	listCollaborationsCommand(const System& system);
	void execute() override;

};

class AddUserCommand : public Command
{
public:
	AddUserCommand(const System& system, const MyString& collaborationName, const MyString& username);
	void execute() override;
private:
	MyString collaborationName;
	MyString username;
};

class AssignTaskCommand : public Command
{
public:
	AssignTaskCommand(const System& system, const MyString& collaborationName, const MyString& username, const MyString& taskName, const std::tm& due_date, const MyString& desc);
	void execute() override;
private:
	MyString collaborationName;
	MyString username;
	MyString taskName;
	std::tm due_date;
	MyString desc;

};