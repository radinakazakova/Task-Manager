#include "AddTaskCommand.h"
#include "TimeManagement.h"
#include "Exceptions.h"
#include <iostream>

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