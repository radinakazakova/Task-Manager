#include "StartTaskCommand.h"
#include "Exceptions.h"

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