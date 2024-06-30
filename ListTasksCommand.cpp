#include "ListTasksCommand.h"

#include "CommandUtils.h"
#include "Exceptions.h"
#include "TimeManagement.h"

ListTasksCommand::ListTasksCommand(Session& context, const std::tm& date) : AuthCommand(context), date(date)
{
	hasDate = true;
}

ListTasksCommand::ListTasksCommand(Session& context) : AuthCommand(context) {}

ListTasksCommand::ListTasksCommand(Session& context, const MyString& collaborationName) : AuthCommand(context), collaborationName(collaborationName)
{
	hasCollabName = true;
}

void ListTasksCommand::executeWithUser(User* user) {
	if (hasCollabName) {
		CollaborationRepository& collabRepo = context.getCollaborationRepository();
		Collaboration* collab = collabRepo.findByName(collaborationName);

		if (collab && collab->hasUser(user)) {
			const auto& collabTasks = collab->getTasks();

			for (size_t i = 0; i < collabTasks.getSize(); i++) {
				if (collabTasks[i].getFirst()->getId() != -1) {
					CommandUtils::printTaskToConsole(collabTasks[i].getFirst());
					std::cout << "Assignee: " << collabTasks[i].getSecond()->getUsername() << "\n\n";
				}
			}
		}
		else
		{
			throw UnauthorizedException("Collaboration not found or no access to such collaboration.");
		}
	}
	else {
		const auto& tasks = user->getTasks();

		for (size_t i = 0; i < tasks.getSize(); i++) {
			if (tasks[i]->getId() != -1) {
				if (!hasDate || (tasks[i]->hasDate() && TimeManagement::isSameDay(tasks[i]->getDueDate(), date))) {
					CommandUtils::printTaskToConsole(tasks[i]);
				}
			}
		}
	}
}