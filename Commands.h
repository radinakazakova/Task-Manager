#pragma once
#include "MyString.h"
#include "Session.h"

//class Command {
//public:
//	Command(Session& context);
//	virtual void execute() = 0;
//	virtual ~Command() = default;
//protected:
//	Session& context;
//};

//class AuthCommand : public Command {
//public:
//	AuthCommand(Session& context);
//	void execute() override final;
//protected:
//	virtual void executeWithUser(User* user) = 0;
//};

//class RegisterCommand : public Command {
//public:
//	RegisterCommand(Session& context, const MyString& username, const MyString& password);
//	void execute() override;
//private:
//	MyString username, password;
//};

//class LoginCommand : public Command {
//public:
//	LoginCommand(Session& context, const MyString& username, const MyString& password);
//	void execute() override;
//private:
//	MyString username, password;
//};

//class LogoutCommand : public AuthCommand {
//public:
//	LogoutCommand(Session& context);
//protected:
//	void executeWithUser(User* user) override;
//};

//class AddTaskCommand : public AuthCommand {
//public:
//	AddTaskCommand(Session& context, const MyString& name, const std::tm& due_date, const MyString& description);
//	AddTaskCommand(Session& context, const MyString& name, const MyString& description);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	MyString name, description;
//	std::tm due_date;
//	bool hasDueDate = false;
//};

//class UpdateTaskNameCommand : public AuthCommand {
//public:
//	UpdateTaskNameCommand(Session& context, const int id, const MyString& name);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id;
//	MyString name;
//};

//class StartTaskCommand : public AuthCommand {
//public:
//	StartTaskCommand(Session& context, const int id);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id;
//};

//class FinishTaskCommand : public AuthCommand {
//public:
//	FinishTaskCommand(Session& context, const int id);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id;
//};

//class DeleteTaskCommand : public AuthCommand {
//public:
//	DeleteTaskCommand(Session& context, const int id);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id;
//};

//class AddTaskToDashboardCommand : public AuthCommand {
//public:
//	AddTaskToDashboardCommand(Session& context, const int id);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id;
//};

//class RemoveTaskFromDashboardCommand : public AuthCommand {
//public:
//	RemoveTaskFromDashboardCommand(Session& context, const int id);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id;
//};

//class AddCollaborationCommand : public AuthCommand {
//public:
//	AddCollaborationCommand(Session& context, const MyString& name);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	MyString name;
//};

//class DeleteCollaborationCommand : public AuthCommand {
//public:
//	DeleteCollaborationCommand(Session& context, const MyString& name);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	MyString name;
//};

//class AddUserCommand : public AuthCommand {
//public:
//	AddUserCommand(Session& context, const MyString& collaborationName, const MyString& username);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	MyString collaborationName, username;
//};

//class AssignTaskCommand : public AuthCommand {
//public:
//	AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
//		const MyString& taskName, const std::tm& due_date, const MyString& desc);
//	AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
//		const MyString& taskName, const MyString& desc);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	MyString collaborationName, username, taskName, desc;
//	std::tm due_date;
//	bool hasDate = false;
//};

//class GetTaskCommand : public AuthCommand {
//public:
//	GetTaskCommand(Session& context, const int id);
//	GetTaskCommand(Session& context, const MyString& name);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	int id = -1;
//	MyString name;
//};

//class ListTasksCommand : public AuthCommand {
//public:
//	ListTasksCommand(Session& context, const std::tm& date);
//	ListTasksCommand(Session& context);
//	ListTasksCommand(Session& context, const MyString& collaborationName);
//protected:
//	void executeWithUser(User* user) override;
//private:
//	std::tm date;
//	MyString collaborationName;
//	bool hasDate = false;
//	bool hasCollabName = false;
//};

//class ListCompletedTasksCommand : public AuthCommand {
//public:
//	ListCompletedTasksCommand(Session& context);
//protected:
//	void executeWithUser(User* user) override;
//};

//class ListDashboardCommand : public AuthCommand {
//public:
//	ListDashboardCommand(Session& context);
//protected:
//	void executeWithUser(User* user) override;
//};

//class ListCollaborationsCommand : public AuthCommand {
//public:
//	ListCollaborationsCommand(Session& context);
//protected:
//	void executeWithUser(User* user) override;
//};