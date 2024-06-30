#include "UpdateTaskNameCommand.h"
#include "Exceptions.h"

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