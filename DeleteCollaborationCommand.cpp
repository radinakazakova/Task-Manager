#include "DeleteCollaborationCommand.h"

#include "Exceptions.h"
DeleteCollaborationCommand::DeleteCollaborationCommand(Session& context, const MyString& name) : AuthCommand(context), name(name) {}

void DeleteCollaborationCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	Collaboration* collab = collabRepo.findByName(name);

	if (!collab) {
		throw NotFoundException("Collaboration not found.");
	}

	if (collab->getCreator() != user) {
		throw UnauthorizedException("You cannot delete this collaboration.");
	}

	const auto& collabTasks = collab->getTasks();
	for (size_t i = 0; i < collabTasks.getSize(); i++) {
		collabTasks[i].getFirst()->setInvalidId();
	}

	collab->setInvalidId();
	std::cout << "Collaboration deleted!" << std::endl;
}