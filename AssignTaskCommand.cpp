#include "AssignTaskCommand.h"

#include "Exceptions.h"

AssignTaskCommand::AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
                                     const MyString& taskName, const std::tm& due_date, const MyString& desc)
	: AuthCommand(context), collaborationName(collaborationName), username(username),
	taskName(taskName), desc(desc), due_date(due_date), hasDate(true) {
}

AssignTaskCommand::AssignTaskCommand(Session& context, const MyString& collaborationName, const MyString& username,
	const MyString& taskName, const MyString& desc)
	: AuthCommand(context), collaborationName(collaborationName), username(username),
	taskName(taskName), desc(desc), due_date({}) {
}

void AssignTaskCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	UserRepository& userRepo = context.getUserRepository();

	Collaboration* collab = collabRepo.findByName(collaborationName);
	if (!collab) throw NotFoundException("Collaboration not found.");

	User* assignee = userRepo.findByUsername(username);
	if (!assignee) throw NotFoundException("User not found.");

	Task* newTask = hasDate ? new Task(taskName, due_date, desc) : new Task(taskName, desc);

	try {
		collab->tryAssignTask(user, assignee, newTask);

		TaskRepository& taskRepo = context.getTaskRepository();
		taskRepo.addTask(newTask);
		std::cout << "Task assigned successfully in collaboration!" << std::endl;
	}
	catch (const BusinessException& e) {
		delete newTask;
		throw;
	}
}