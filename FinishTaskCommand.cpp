#include "FinishTaskCommand.h"
#include "Exceptions.h"

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