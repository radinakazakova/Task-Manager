#include "AddTaskToDashboardCommand.h"

AddTaskToDashboardCommand::AddTaskToDashboardCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void AddTaskToDashboardCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);

	user->tryAddTaskToDashboard(task);
	std::cout << "Task added to dashboard!" << std::endl;
}