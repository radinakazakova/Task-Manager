#include "Commands.h"
#include "TimeManagement.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <climits>

namespace {
	void printTaskStatusToConsole(TaskStatus stat) {
		switch (stat) {
		case TaskStatus::ON_HOLD:    std::cout << "ON HOLD"; break;
		case TaskStatus::DONE:       std::cout << "DONE"; break;
		case TaskStatus::IN_PROCESS: std::cout << "IN PROCESS"; break;
		case TaskStatus::OVERDUE:    std::cout << "OVERDUE"; break;
		}
		std::cout << std::endl;
	}

	void printTaskToConsole(const Task* task) {
		if (!task)
		{
			return;
		}

		std::cout << "Task name: " << task->getName() << std::endl;
		std::cout << "Task ID: " << task->getId() << std::endl;

		if (task->hasDate()) {
			std::cout << "Due date: " << std::put_time(&task->getDueDate(), "%Y-%m-%d %H:%M:%S") << std::endl;
		}

		std::cout << "Status: ";
		printTaskStatusToConsole(task->getStatus());
		std::cout << "Task description: " << task->getDescription() << std::endl;
	}
}

Command::Command(Session& context) : context(context) {}

AuthCommand::AuthCommand(Session& context) : Command(context) {}

void AuthCommand::execute() {
	User* user = context.getLoggedInUser();

	if (!user)
	{
		throw AuthException("You are not logged in.");
	}

	executeWithUser(user);
}

RegisterCommand::RegisterCommand(Session& context, const MyString& username, const MyString& password)
	: Command(context), username(username), password(password) {
}

void RegisterCommand::execute() {
	if (context.getLoggedInUser() != nullptr) {
		throw AuthException("You are already logged in. Please logout first to register a new account.");
	}

	if (username.getLength() == 0 || password.getLength() == 0) {
		throw InvalidStateException("Please input password and/or username.");
	}

	UserRepository& userRepo = context.getUserRepository();

	if (userRepo.findByUsername(username) == nullptr) {
		User* newUser = new User(username, password);
		userRepo.addUser(newUser);
		std::cout << "Successfully created account!" << std::endl;
	}
	else {
		throw DuplicateException("Username already taken.");
	}
}

LoginCommand::LoginCommand(Session& context, const MyString& username, const MyString& password)
	: Command(context), username(username), password(password) {
}

void LoginCommand::execute() {
	if (context.getLoggedInUser() != nullptr) {
		throw AuthException("You are already logged in. Please logout first to log into another account.");
	}

	UserRepository& userRepo = context.getUserRepository();
	User* user = userRepo.findByUsername(username);

	if (user != nullptr && user->checkPassword(password)) {
		std::time_t currTime = std::time(nullptr);
		std::tm loggedInTime;
		localtime_s(&loggedInTime, &currTime);

		context.login(user, loggedInTime);
		user->syncTasksAndDashboard(loggedInTime);

		std::cout << "Login successful" << std::endl;
	}
	else {
		throw AuthException("Login unsuccessful. Invalid credentials.");
	}
}

LogoutCommand::LogoutCommand(Session& context) : AuthCommand(context) {}

void LogoutCommand::executeWithUser(User* user) {
	context.logout();
	std::cout << "Logged out successfully!" << std::endl;
}

AddTaskCommand::AddTaskCommand(Session& context, const MyString& name, const std::tm& due_date, const MyString& description)
	: AuthCommand(context), name(name), description(description), due_date(due_date), hasDueDate(true) {
}

AddTaskCommand::AddTaskCommand(Session& context, const MyString& name, const MyString& description)
	: AuthCommand(context), name(name), description(description), due_date({}) {
}

void AddTaskCommand::executeWithUser(User* user) {
	Task* newTask = hasDueDate ? new Task(name, due_date, description) : new Task(name, description);

	try {
		user->tryAddTask(newTask);

		TaskRepository& taskRepo = context.getTaskRepository();
		taskRepo.addTask(newTask);

		if (hasDueDate && TimeManagement::isSameDay(due_date, context.getLoggedInTime())) {
			if (!user->isTaskInDashboard(newTask)) {
				user->tryAddTaskToDashboard(newTask);
			}
		}
		std::cout << "Task added successfully!" << std::endl;
	}
	catch (const BusinessException& e) {
		delete newTask;
		throw;
	}
}

UpdateTaskNameCommand::UpdateTaskNameCommand(Session& context, const int id, const MyString& name) : AuthCommand(context), id(id), name(name) {}

void UpdateTaskNameCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);
	if (task) {
		task->changeName(name);
		std::cout << "Task name updated!" << std::endl;
	}
	else
	{
		throw NotFoundException("Task not found.");
	}
}

StartTaskCommand::StartTaskCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void StartTaskCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);

	if (!task) {
		throw NotFoundException("Task not found.");
	}

	if (task->getStatus() == TaskStatus::DONE) {
		throw InvalidStateException("Cannot start task (task is already done).");
	}

	task->changeStatus(TaskStatus::IN_PROCESS);
	std::cout << "Task started!" << std::endl;
}

FinishTaskCommand::FinishTaskCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void FinishTaskCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);
	if (task) {
		task->changeStatus(TaskStatus::DONE);

		user->tryRemoveTaskFromDashboard(id);

		std::cout << "Task finished!" << std::endl;
	}
	else
	{
		throw NotFoundException("Task not found.");
	}
}

DeleteTaskCommand::DeleteTaskCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void DeleteTaskCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);
	if (task) {
		user->tryDeleteTask(id);
		task->setInvalidId();
		std::cout << "Task deleted!" << std::endl;
	}
	else
	{
		throw NotFoundException("Task not found.");
	}
}

AddTaskToDashboardCommand::AddTaskToDashboardCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void AddTaskToDashboardCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);

	user->tryAddTaskToDashboard(task);
	std::cout << "Task added to dashboard!" << std::endl;
}

RemoveTaskFromDashboardCommand::RemoveTaskFromDashboardCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void RemoveTaskFromDashboardCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);
	if (!task || !user->isTaskInDashboard(task)) {
		throw NotFoundException("Task is not on your dashboard.");
	}

	user->tryRemoveTaskFromDashboard(id);
	std::cout << "Task removed from dashboard!" << std::endl;
}

AddCollaborationCommand::AddCollaborationCommand(Session& context, const MyString& name) : AuthCommand(context), name(name) {}

void AddCollaborationCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();

	if (collabRepo.findByName(name) != nullptr) {
		throw DuplicateException("A collaboration with this name already exists!");
	}

	Collaboration* newCollab = new Collaboration(name, user);
	collabRepo.addCollaboration(newCollab);

	std::cout << "Collaboration added successfully!" << std::endl;
}

DeleteCollaborationCommand::DeleteCollaborationCommand(Session& context, const MyString& name) : AuthCommand(context), name(name) {}

void DeleteCollaborationCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	Collaboration* collab = collabRepo.findByName(name);

	if (!collab) {
		throw NotFoundException("Collaboration not found.");
	}

	if (collab->getCreator() != user) {
		throw UnauthorizedException("You cannot delete this collaboration.");
	}

	const auto& collabTasks = collab->getTasks();
	for (size_t i = 0; i < collabTasks.getSize(); i++) {
		collabTasks[i].getFirst()->setInvalidId();
	}

	collab->setInvalidId();
	std::cout << "Collaboration deleted!" << std::endl;
}

AddUserCommand::AddUserCommand(Session& context, const MyString& collaborationName, const MyString& username)
	: AuthCommand(context), collaborationName(collaborationName), username(username) {
}

void AddUserCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	UserRepository& userRepo = context.getUserRepository();

	Collaboration* collab = collabRepo.findByName(collaborationName);
	User* targetUser = userRepo.findByUsername(username);

	if (!collab) {
		throw NotFoundException("Collaboration not found.");
	}

	if (!targetUser) {
		throw NotFoundException("User not found.");
	}

	collab->tryAddUser(user, targetUser);
	std::cout << "User added to collaboration!" << std::endl;
}

AssignTaskCommand::AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
	const MyString& taskName, const std::tm& due_date, const MyString& desc)
	: AuthCommand(context), collaborationName(collaborationName), username(username),
	taskName(taskName), desc(desc), due_date(due_date), hasDate(true) {
}

AssignTaskCommand::AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
	const MyString& taskName, const MyString& desc)
	: AuthCommand(context), collaborationName(collaborationName), username(username),
	taskName(taskName), desc(desc), due_date({}) {
}

void AssignTaskCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	UserRepository& userRepo = context.getUserRepository();

	Collaboration* collab = collabRepo.findByName(collaborationName);
	if (!collab) throw NotFoundException("Collaboration not found.");

	User* assignee = userRepo.findByUsername(username);
	if (!assignee) throw NotFoundException("User not found.");

	Task* newTask = hasDate ? new Task(taskName, due_date, desc) : new Task(taskName, desc);

	try {
		collab->tryAssignTask(user, assignee, newTask);

		TaskRepository& taskRepo = context.getTaskRepository();
		taskRepo.addTask(newTask);
		std::cout << "Task assigned successfully in collaboration!" << std::endl;
	}
	catch (const BusinessException& e) {
		delete newTask;
		throw;
	}
}

GetTaskCommand::GetTaskCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

GetTaskCommand::GetTaskCommand(Session& context, const MyString& name) : AuthCommand(context), name(name) {}

void GetTaskCommand::executeWithUser(User* user) {
	if (id != -1) {
		Task* taskToPrint = user->getTaskById(id);

		if (taskToPrint) {
			printTaskToConsole(taskToPrint);
		}
		else {
			throw NotFoundException("Task not found.");
		}
	}
	else {
		bool foundAny = false;
		const auto& tasks = user->getTasks();

		for (size_t i = 0; i < tasks.getSize(); i++) {
			if (tasks[i]->getId() != -1 && tasks[i]->getName() == name) {
				printTaskToConsole(tasks[i]);
				std::cout << "------------------------" << std::endl;
				foundAny = true;
			}
		}

		if (!foundAny) {
			throw NotFoundException("Task not found.");
		}
	}
}

ListTasksCommand::ListTasksCommand(Session& context, const std::tm& date) : AuthCommand(context), date(date)
{
	hasDate = true;
}

ListTasksCommand::ListTasksCommand(Session& context) : AuthCommand(context) {}

ListTasksCommand::ListTasksCommand(Session& context, const MyString& collaborationName) : AuthCommand(context), collaborationName(collaborationName)
{
	hasCollabName = true;
}

void ListTasksCommand::executeWithUser(User* user) {
	if (hasCollabName) {
		CollaborationRepository& collabRepo = context.getCollaborationRepository();
		Collaboration* collab = collabRepo.findByName(collaborationName);

		if (collab && collab->hasUser(user)) {
			const auto& collabTasks = collab->getTasks();

			for (size_t i = 0; i < collabTasks.getSize(); i++) {
				if (collabTasks[i].getFirst()->getId() != -1) {
					printTaskToConsole(collabTasks[i].getFirst());
					std::cout << "Assignee: " << collabTasks[i].getSecond()->getUsername() << "\n\n";
				}
			}
		}
		else
		{
			throw UnauthorizedException("Collaboration not found or no access to such collaboration.");
		}
	}
	else {
		const auto& tasks = user->getTasks();

		for (size_t i = 0; i < tasks.getSize(); i++) {
			if (tasks[i]->getId() != -1) {
				if (!hasDate || (tasks[i]->hasDate() && TimeManagement::isSameDay(tasks[i]->getDueDate(), date))) {
					printTaskToConsole(tasks[i]);
				}
			}
		}
	}
}

ListCompletedTasksCommand::ListCompletedTasksCommand(Session& context) : AuthCommand(context) {}

void ListCompletedTasksCommand::executeWithUser(User* user) {
	const auto& tasks = user->getTasks();

	for (size_t i = 0; i < tasks.getSize(); i++) {
		if (tasks[i]->getId() != -1 && tasks[i]->getStatus() == TaskStatus::DONE) {
			printTaskToConsole(tasks[i]);
		}
	}
}

ListDashboardCommand::ListDashboardCommand(Session& context) : AuthCommand(context) {}

void ListDashboardCommand::executeWithUser(User* user) {
	const auto& dashboard = user->getDashboard();

	for (size_t i = 0; i < dashboard.getSize(); i++) {
		if (dashboard[i]->getId() != -1)
		{
			printTaskToConsole(dashboard[i]);
		}
	}
}

ListCollaborationsCommand::ListCollaborationsCommand(Session& context) : AuthCommand(context) {}

void ListCollaborationsCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	const auto& collabs = collabRepo.getAllCollaborations();

	for (size_t i = 0; i < collabs.getSize(); i++) {
		if (collabs[i]->getId() != -1 && collabs[i]->hasUser(user)) {
			std::cout << collabs[i]->getName() << std::endl;
		}
	}
}