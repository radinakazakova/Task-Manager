#include "RemoveTaskFromDashboardCommand.h"
#include "Exceptions.h"

RemoveTaskFromDashboardCommand::RemoveTaskFromDashboardCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

void RemoveTaskFromDashboardCommand::executeWithUser(User* user) {
	Task* task = user->getTaskById(id);
	if (!task || !user->isTaskInDashboard(task)) {
		throw NotFoundException("Task is not on your dashboard.");
	}

	user->tryRemoveTaskFromDashboard(id);
	std::cout << "Task removed from dashboard!" << std::endl;
}