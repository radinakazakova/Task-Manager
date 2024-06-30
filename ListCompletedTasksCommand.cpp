#include "ListCompletedTasksCommand.h"

#include "CommandUtils.h"

ListCompletedTasksCommand::ListCompletedTasksCommand(Session& context) : AuthCommand(context) {}

void ListCompletedTasksCommand::executeWithUser(User* user) {
	const auto& tasks = user->getTasks();

	for (size_t i = 0; i < tasks.getSize(); i++) {
		if (tasks[i]->getId() != -1 && tasks[i]->getStatus() == TaskStatus::DONE) {
			CommandUtils::printTaskToConsole(tasks[i]);
		}
	}
}
