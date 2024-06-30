#include "UpdateTaskDescriptionCommand.h"
#include "Exceptions.h"
#include <iostream>

UpdateTaskDescriptionCommand::UpdateTaskDescriptionCommand(Session& context, int id, const MyString& description)
	: AuthCommand(context), id(id), description(description)
{
}

void UpdateTaskDescriptionCommand::executeWithUser(User* user)
{
	Task* task = user->getTaskById(id);

	if (task) {
		task->changeDescription(description);
		std::cout << "Task description updated successfully!" << std::endl;
	} else {
		throw NotFoundException("Task not found.");
	}
}