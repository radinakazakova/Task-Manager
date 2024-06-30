#include "DeleteTaskCommand.h"
#include "Exceptions.h"

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