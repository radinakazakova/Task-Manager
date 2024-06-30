#include "GetTaskCommand.h"

#include "CommandUtils.h"
#include "Exceptions.h"

GetTaskCommand::GetTaskCommand(Session& context, const int id) : AuthCommand(context), id(id) {}

GetTaskCommand::GetTaskCommand(Session& context, const MyString& name) : AuthCommand(context), name(name) {}

void GetTaskCommand::executeWithUser(User* user) {
	if (id != -1) {
		Task* taskToPrint = user->getTaskById(id);

		if (taskToPrint) {
			CommandUtils::printTaskToConsole(taskToPrint);
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
				CommandUtils::printTaskToConsole(tasks[i]);
				std::cout << "------------------------" << std::endl;
				foundAny = true;
			}
		}

		if (!foundAny) {
			throw NotFoundException("Task not found.");
		}
	}
}